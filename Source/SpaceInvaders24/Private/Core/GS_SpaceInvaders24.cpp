#include "Core/GS_SpaceInvaders24.h"

#include "Actors/Bunker.h"
#include "Actors/Crystal.h"
#include "Actors/Enemy.h"
#include "Actors/GamePreviewActor.h"
#include "Actors/LaserTank.h"
#include "Actors/Shot.h"
#include "Components/GameTimeManager.h"
#include "Components/SwarmMind.h"
#include "Core/PC_SpaceInvaders24.h"
#include "GAS/CustomAbilitySystemComponent.h"
#include "GAS/GASEnums.h"
#include "GameFramework/PlayerState.h"
#include "Internationalization/StringTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "UI/GUI.h"
#include "Utils/HighScoreSaveGame.h"


FString AGS_SpaceInvaders24::GetLocalizatedString(FString Key) const {
	if (ChosenLanguage == nullptr) {
		return Key;
	}

	FName StringTableId = ChosenLanguage->GetStringTableId();
	FText LocalizatedText = UKismetTextLibrary::TextFromStringTable(StringTableId, Key);
	FString LocalizatedString = LocalizatedText.ToString();

	if (LocalizatedString == "<MISSING STRING TABLE ENTRY>") {
		return Key;
	}

	return LocalizatedString;
}

void AGS_SpaceInvaders24::SetGUIReference(UGUI *GUIReference) {
	GUI = GUIReference;

	GUI->OnClickPlay.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnGUIClickPlay);
	GUI->OnReadySetGoFinished.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnReadySetGoFinished);
	GUI->OnPassLevelFinished.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnPassLevelFinished);
	GUI->OnGameOverFinished.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnGameOverFinished);
	GUI->OnLevelClearFinished.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnLevelClearFinished);
}

AGS_SpaceInvaders24::AGS_SpaceInvaders24() {
	PrimaryActorTick.bCanEverTick = true;

	GameTimeManager = CreateDefaultSubobject<UGameTimeManager>(TEXT("Game Time Manager"));
	GameTimeManager->OnTimeStateFinished.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnTimeStateFinished);

	SwarmMind = CreateDefaultSubobject<USwarmMind>(TEXT("Swarm Mind"));
	SwarmMind->EnemyDiedEvent.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnEnemyDiedEvent);
	SwarmMind->EnemiesCantGoLower.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnEnemiesCantGoLower);
	SwarmMind->KilledAllEnemies.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnKilledAllEnemies);
}

void AGS_SpaceInvaders24::SpawnPlayer() {
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector PlayerWorldPosition = TexelToWorldPos(PlayerSpawnPosition);
	Player = GetWorld()->SpawnActor<ALaserTank>(PlayerClass, PlayerWorldPosition, GetGameObjectOrientation(), ActorSpawnParams);
	Player->ManualInitialize();
	Player->OnStartDying.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnPlayerStartDying);
	Player->OnFinallyDie.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnPlayerFinallyDie);

	if (PlayerArray.Num() > 0) {
		APlayerController *PC = PlayerArray[0]->GetPlayerController();
		if (PC->GetPawn<ALaserTank>() == nullptr) {
			PC->Possess(Player);
		}
	}
}

void AGS_SpaceInvaders24::SpawnBunkers() {
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 i = 0; i < BunkerCoordinates.Num(); i++) {
		FIntPoint BunkerTexelCoordinate = BunkerCoordinates[i];
		FVector BunkerWorldPosition = TexelToWorldPos(BunkerTexelCoordinate);
		ABunker *NewBunker = GetWorld()->SpawnActor<ABunker>(BunkerClass, BunkerWorldPosition, GetGameObjectOrientation(), ActorSpawnParams);
		NewBunker->ManualInitialize();
		Bunkers.Add(NewBunker);
	}
}

void AGS_SpaceInvaders24::SpawnUFO() {
	if (UFOClass == nullptr) {
		return;
	}

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector UFOWorldPosition = TexelToWorldPos(UFOSpawnPosition);
	UFO = GetWorld()->SpawnActor<AEnemy>(UFOClass, UFOWorldPosition, GetGameObjectOrientation(), ActorSpawnParams);
	UFO->ManualInitialize(FIntPoint(-1, -1));

	UFO->OnTouchLimit.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnUFOTouchBorder);
	UFO->OnDie.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnEnemyDiedEvent);
}

void AGS_SpaceInvaders24::ResetGame(bool HardReset, int32 LevelToLoad) {
	LastUFOAppearance = 0;
	UFO->Kill(true);

	if (HardReset) {
		AlreadyEmittedNewHighScore = false;
	}

	GameTimeManager->ManualReset();
	SwarmMind->ManualReset(LevelToLoad);

	Player->ManualReset(HardReset);
	Player->SetTexelPosition(PlayerSpawnPosition, false);

	for (int32 i = 0; i < Bunkers.Num(); i++) {
		ABunker *Bunker = Bunkers[i];
		Bunker->ManualReset();
	}

	for (int32 i = 0; i < Crystals.Num(); i++) {
		Crystals[i]->Destroy();
	}
	Crystals.Empty();

	for (int32 i = 0; i < Shots.Num(); i++) {
		Shots[i]->Destroy();
	}
	Shots.Empty();
}


void AGS_SpaceInvaders24::UFOAppear() {
	UFO->ManualReset(UFOSpawnPosition);
	UFO->SetTexelVelocity(FVector2D(UFOMovementSpeed * 60.f, 0));
}

void AGS_SpaceInvaders24::InitNewLevel() {
	Player->GetCustomAbilitySystemComponent()->AddToAttributeValueByEnum(EPlayerAttribute::Level, 1);
	Player->GetCustomAbilitySystemComponent()->SetAttributeValueByEnum(EPlayerAttribute::Crystals, 0);
	int32 NewLevel = FMath::RoundToInt(Player->GetCustomAbilitySystemComponent()->GetAttributeValueByEnum(EPlayerAttribute::Level));

	ResetGame(false, NewLevel);
	SetNewState(EGameState::PASSING_LEVEL);
}

void AGS_SpaceInvaders24::OnTimeStateFinished() { SetNewState(EGameState::PLAYING_FORWARD); }

void AGS_SpaceInvaders24::OnEnemyDiedEvent(AEnemy *EnemyDied, int32 PointsGiven) {
	Player->GetCustomAbilitySystemComponent()->AddToAttributeValueByEnum(EPlayerAttribute::Points, PointsGiven);
	int32 CurrentPlayerPoints = static_cast<int32>(Player->GetCustomAbilitySystemComponent()->GetAttributeValueByEnum(EPlayerAttribute::Points));
	if (CurrentPlayerPoints > HighScore) {
		HighScore = CurrentPlayerPoints;
		OnNewHighScore.Broadcast(HighScore);
		if (!AlreadyEmittedNewHighScore) {
			AlreadyEmittedNewHighScore = true;
			GUI->K2_ShowNewHIghScoreSign();
		}
	}
}

void AGS_SpaceInvaders24::OnEnemiesCantGoLower() {
	if (Player->CanRevive()) {
		MoveEnemiesToUp = true;
	}
	Player->Kill(false);
}

void AGS_SpaceInvaders24::OnKilledAllEnemies() {
	for (int32 i = 0; i < Shots.Num(); i++) {
		Shots[i]->Destroy();
	}
	Shots.Empty();

	SetNewState(EGameState::LEVEL_CLEAR);
}

void AGS_SpaceInvaders24::OnShotHit(AShot *Shot) { Shots.Remove(Shot); }

void AGS_SpaceInvaders24::OnCrystalHit(ACrystal *Crystal, bool AddCrystalCount) {
	if (AddCrystalCount) {
		Player->GetCustomAbilitySystemComponent()->AddToAttributeValueByEnum(EPlayerAttribute::Crystals, 1);
	}
	Crystals.Remove(Crystal);
}

void AGS_SpaceInvaders24::OnUFOTouchBorder(EDirection Direction) {
	if (Direction == EDirection::RIGHT) {
		UFO->Kill(true);
	}
}

void AGS_SpaceInvaders24::OnPlayerStartDying() {
	for (int32 i = 0; i < Shots.Num(); i++) {
		Shots[i]->Destroy();
	}
	Shots.Empty();

	SetNewState(EGameState::DYING);
}

void AGS_SpaceInvaders24::OnPlayerFinallyDie() {
	if (Player->CanRevive()) {
		Player->Revive(true);
		if (MoveEnemiesToUp) {
			MoveEnemiesToUp = false;
			SwarmMind->MoveEnemiesToUp();
		}

		SetNewState(EGameState::READY_SET_GO);

	} else {

		if (IsHighScoredPlay()) {
			UHighScoreSaveGame *HighScoreSaveGame = Cast<UHighScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UHighScoreSaveGame::StaticClass()));
			HighScoreSaveGame->HighScore = HighScore;
			UGameplayStatics::SaveGameToSlot(HighScoreSaveGame, "SaveGame", 0);
		}

		SetNewState(EGameState::GAME_OVER);
	}
}


void AGS_SpaceInvaders24::OnGUIClickPlay() { SetNewState(EGameState::PASSING_LEVEL); }

void AGS_SpaceInvaders24::OnReadySetGoFinished() { SetNewState(EGameState::PLAYING_FORWARD); }

void AGS_SpaceInvaders24::OnPassLevelFinished() { SetNewState(EGameState::READY_SET_GO); }

void AGS_SpaceInvaders24::OnGameOverFinished() {
	ResetGame(true);
	SetNewState(EGameState::IN_MENU);
}

void AGS_SpaceInvaders24::OnLevelClearFinished() { InitNewLevel(); }

void AGS_SpaceInvaders24::BeginPlay() {
	Super::BeginPlay();

	if (UGameplayStatics::DoesSaveGameExist("SaveGame", 0)) {
		UHighScoreSaveGame *HighScoreSaveGame = Cast<UHighScoreSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveGame", 0));
		if (HighScoreSaveGame != nullptr) {
			HighScore = HighScoreSaveGame->HighScore;
		}
	}

	GamePreviewActor = Cast<AGamePreviewActor>(UGameplayStatics::GetActorOfClass(this, AGamePreviewActor::StaticClass()));
	GamePreviewActor->ManualInitialize();

	// Initialize Swarm and spawns all enemies
	SwarmMind->ManualInitialize();

	SpawnUFO();
	SpawnPlayer();
	SpawnBunkers();

	ResetGame(true);

	SetNewState(EGameState::IN_MENU);
}

void AGS_SpaceInvaders24::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	GameTimeManager->ManualTick(DeltaTime);

	Player->ManualTick(GetLastDeltaTime());

	SwarmMind->ManualTick(GetLastCrystalDeltaTime(), GetCrystalTotalSeconds());

	if (GameState == EGameState::PLAYING_FORWARD || GameState == EGameState::PLAYING_SLOW_TIME_DOWN) {
		if (GetCrystalTotalSeconds() - LastUFOAppearance > UFOSecondsPerAppearance) {
			LastUFOAppearance += UFOSecondsPerAppearance;
			UFOAppear();
		} else if (UFO->IsAlive()) {
			UFO->ApplyVelocity(GetLastCrystalDeltaTime());
		}
	}


	// I need to iterate a copy because this ManualTick could make that shot dissapear and the iteration will be wrong
	TArray<AShot *> ShotsCopy = Shots;
	for (AShot *Shot : ShotsCopy) {
		Shot->ManualTick(GetLastDeltaTime());
	}
}

TSubclassOf<AShot> AGS_SpaceInvaders24::GetShotClass(EShotType ShotClassEnum) const {
	const TSubclassOf<AShot> *ShotClass = ShotsClasses.Find(ShotClassEnum);
	if (ShotClass == nullptr) {
		return nullptr;
	}
	return *ShotClass;
}

void AGS_SpaceInvaders24::SetNewState(EGameState NewGameState) {
	GameState = NewGameState;

	GameTimeManager->OnNewGameState(NewGameState);
	SwarmMind->OnNewGameState(NewGameState);

	OnNewGameState.Broadcast(NewGameState);
}

EGameState AGS_SpaceInvaders24::GetGameState() const { return GameState; }

int32 AGS_SpaceInvaders24::GetHighScore() const { return HighScore; }

bool AGS_SpaceInvaders24::IsHighScoredPlay() const { return AlreadyEmittedNewHighScore; }

void AGS_SpaceInvaders24::OnPlayerControllerConnected(APlayerController *PC) {
	if (Player != nullptr && PC->GetPawn<ALaserTank>() == nullptr) {
		PC->Possess(Player);
	}
}

void AGS_SpaceInvaders24::ReportNewShot(AShot *NewShot) {
	NewShot->OnHit.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnShotHit);
	Shots.Add(NewShot);
}

void AGS_SpaceInvaders24::ReportNewCrystal(ACrystal *NewCrystal) {
	NewCrystal->OnHit.AddUniqueDynamic(this, &AGS_SpaceInvaders24::OnCrystalHit);
	Crystals.Add(NewCrystal);
}

const TArray<AShot *> &AGS_SpaceInvaders24::GetShots() const { return Shots; }

#pragma region // Wrapped functions from GamePreviewActor
FIntPoint AGS_SpaceInvaders24::GetMapSize() const { return GamePreviewActor->GetMapSize(); }

FIntPoint AGS_SpaceInvaders24::WorldToTexelPos(FVector WorldPos) const { return GamePreviewActor->WorldToTexelPos(WorldPos); }

FVector AGS_SpaceInvaders24::TexelToWorldPos(FIntPoint TexelPos) const { return GamePreviewActor->TexelToWorldPos(TexelPos); }

FVector AGS_SpaceInvaders24::GetForward() const { return GamePreviewActor->GetForward(); }

FVector AGS_SpaceInvaders24::GetUp() const { return GamePreviewActor->GetUp(); }

FRotator AGS_SpaceInvaders24::GetGameObjectOrientation() const { return GamePreviewActor->GetGameObjectOrientation(); }
#pragma endregion
#pragma region // Wrapped functions from GameTimeManager
float AGS_SpaceInvaders24::GetDurationOfLongestTimeState() { return GameTimeManager->GetDurationOfLongestTimeState(); }

float AGS_SpaceInvaders24::GetNormalGameTotalSeconds() { return GameTimeManager->GetNormalGameTotalSeconds(); }

float AGS_SpaceInvaders24::GetCrystalTotalSeconds() { return GameTimeManager->GetCrystalTotalSeconds(); }

float AGS_SpaceInvaders24::GetLastDeltaTime() { return GameTimeManager->GetLastDeltaTime(); }

float AGS_SpaceInvaders24::GetLastCrystalDeltaTime() { return GameTimeManager->GetLastCrystalDeltaTime(); }
#pragma endregion