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
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"


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

	if (PlayerArray.Num() > 0) {
		APlayerController *PC = PlayerArray[0]->GetPlayerController();
		if (PC->GetPawn() == nullptr) {
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

void AGS_SpaceInvaders24::ResetGame() {
	Level = 0;
	Lives = 3;
	Points = 0;

	LastUFOAppearance = 0;

	SpawnUFO();
	SpawnPlayer();
	// SpawnBunkers();

	GameState = EGameState::PLAYING_FORWARD;
	GameTimeManager->SetNewState(ETimeState::FORWARD);

	SwarmMind->ManualReset(0);


	Player->StartGame();
}


void AGS_SpaceInvaders24::UFOAppear() {
	UFO->ManualReset(UFOSpawnPosition);
	UFO->SetTexelVelocity(FVector2D(UFOMovementSpeed * 60.f, 0));
}

void AGS_SpaceInvaders24::OnTimeStateFinished() {
	UKismetSystemLibrary::PrintString(GetWorld(), "On Time State Finished", true, true, FColor::Yellow, 5);
	SetNewState(EGameState::PLAYING_FORWARD);
}

void AGS_SpaceInvaders24::OnEnemyDiedEvent(AEnemy *EnemyDied, int32 PointsGiven) {
	Points += PointsGiven;
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Enemy Died. Points: %d"), Points), true, true, FColor::Green, 5);
}

void AGS_SpaceInvaders24::OnEnemiesCantGoLower() {
	UKismetSystemLibrary::PrintString(GetWorld(), "GAME OVER", true, true, FColor::Red, 5);

	SetNewState(EGameState::GAME_OVER);
}

void AGS_SpaceInvaders24::OnKilledAllEnemies() {
	UKismetSystemLibrary::PrintString(GetWorld(), "OnKilledAllEnemies", true, true, FColor::Red, 5);

	// GetWorld()->GetTimerManager().SetTimer(
	// 	InputTimeHandle, [&]() { Destroy(); }, Duration, false);


	SetNewState(EGameState::PASSING_LEVEL);
}

void AGS_SpaceInvaders24::OnShotHit(AShot *Shot) { Shots.Remove(Shot); }

void AGS_SpaceInvaders24::OnCrystalHit(ACrystal *Crystal, bool AddCrystalCount) {
	if (AddCrystalCount) {
		int32 CurrentCrystalCount = Player->GetCustomAbilitySystemComponent()->GetAttributeValueByEnum(EPlayerAttribute::Crystals);
		Player->GetCustomAbilitySystemComponent()->SetAttributeValueByEnum(EPlayerAttribute::Crystals, CurrentCrystalCount + 1);
	}
	Crystals.Remove(Crystal);
}

void AGS_SpaceInvaders24::OnUFOTouchBorder(EDirection Direction) {
	if (Direction == EDirection::RIGHT) {
		UFO->Kill(true);
	}
}

void AGS_SpaceInvaders24::BeginPlay() {
	Super::BeginPlay();

	GamePreviewActor = Cast<AGamePreviewActor>(UGameplayStatics::GetActorOfClass(this, AGamePreviewActor::StaticClass()));
	GamePreviewActor->ManualInitialize();

	SwarmMind->ManualInitialize();

	ResetGame();
}

void AGS_SpaceInvaders24::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	GameTimeManager->ManualTick(DeltaTime);

	Player->ManualTick(GetLastDeltaTime());

	SwarmMind->ManualTick(GetLastCrystalDeltaTime(), GetCrystalTotalSeconds());

	if ((GameState == EGameState::PLAYING_FORWARD || GameState == EGameState::PLAYING_SLOW_TIME_DOWN) && GetCrystalTotalSeconds() - LastUFOAppearance > UFOSecondsPerAppearance) {
		LastUFOAppearance += UFOSecondsPerAppearance;
		UFOAppear();
	}
	if (UFO->IsAlive()) {
		UFO->ApplyVelocity(GetLastCrystalDeltaTime());
	}


	// I need to iterate a copy because this ManualTick could make that shot dissapear and the iteration will be wrong
	TArray<AShot *> ShotsCopy = Shots;
	for (AShot *Shot : ShotsCopy) {
		Shot->ManualTick(GetLastDeltaTime());
	}
}

void AGS_SpaceInvaders24::SetNewState(EGameState NewGameState) {
	GameState = NewGameState;

	switch (NewGameState) {
	case EGameState::PLAYING_FORWARD:
	case EGameState::PASSING_LEVEL:
		GameTimeManager->SetNewState(ETimeState::FORWARD);
		break;
	case EGameState::PLAYING_SLOW_TIME_DOWN:
		GameTimeManager->SetNewState(ETimeState::SLOW);
		break;
	case EGameState::PLAYING_PAUSED_TIME:
		GameTimeManager->SetNewState(ETimeState::PAUSED);
		break;
	case EGameState::PLAYING_REVERSE:
		GameTimeManager->SetNewState(ETimeState::BACKWARD);
		break;
	default:
		GameTimeManager->SetNewState(ETimeState::IDLE);
		break;
	}

	SwarmMind->OnNewGameState(NewGameState);
}

EGameState AGS_SpaceInvaders24::GetGameState() const { return GameState; }

void AGS_SpaceInvaders24::OnPlayerControllerConnected(APlayerController *PC) {
	if (Player != nullptr && PC->GetPawn() == nullptr) {
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