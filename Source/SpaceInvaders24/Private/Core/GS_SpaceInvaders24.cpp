#include "Core/GS_SpaceInvaders24.h"

#include "Actors/Bunker.h"
#include "Actors/Enemy.h"
#include "Actors/GamePreviewActor.h"
#include "Actors/PWN_Player.h"
#include "Components/GameTimeManager.h"
#include "Kismet/GameplayStatics.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Structs/EnemyRow.h"


AGS_SpaceInvaders24::AGS_SpaceInvaders24() {
	PrimaryActorTick.bCanEverTick = true;

	GameTimeManager = CreateDefaultSubobject<UGameTimeManager>(TEXT("Game Time Manager"));
}

void AGS_SpaceInvaders24::SpawnSwarm() {
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator GameObjectOrientation = GetGameObjectOrientation();

	FIntPoint PositionDelta = TexelCoordOfTopLeftEnemyByLevel[Level];

	for (int32 i = 0; i < EnemyDispositions.Num(); i++) {
		FEnemyRow EnemyRowData = EnemyDispositions[i];
		for (int32 j = 0; j < EnemyRowData.EnemyCount; j++) {
			TSubclassOf<AEnemy> *EnemySubclass = EnemyClasses.Find(EnemyRowData.EnemyType);
			if (EnemySubclass != nullptr) {
				FVector EnemyWorldPosition = TexelToWorldPos(PositionDelta);
				AEnemy *Enemy = GetWorld()->SpawnActor<AEnemy>(*EnemySubclass, EnemyWorldPosition, GameObjectOrientation, ActorSpawnParams);
				Enemies.Add(Enemy);

				PositionDelta.X += SeparationBetweenEnemies.X;
			}
		}
		PositionDelta.X -= SeparationBetweenEnemies.X * EnemyRowData.EnemyCount;
		PositionDelta.Y += SeparationBetweenEnemies.Y;
	}
}

void AGS_SpaceInvaders24::SpawnPlayer() {
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector PlayerWorldPosition = TexelToWorldPos(PlayerSpawnPosition);
	Player = GetWorld()->SpawnActor<APWN_Player>(PlayerClass, PlayerWorldPosition, GetGameObjectOrientation(), ActorSpawnParams);
}

void AGS_SpaceInvaders24::SpawnBunkers() {
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 i = 0; i < BunkerCoordinates.Num(); i++) {
		FIntPoint BunkerTexelCoordinate = BunkerCoordinates[i];
		FVector BunkerWorldPosition = TexelToWorldPos(BunkerTexelCoordinate);
		ABunker *NewBunker = GetWorld()->SpawnActor<ABunker>(BunkerClass, BunkerWorldPosition, GetGameObjectOrientation(), ActorSpawnParams);
		Bunkers.Add(NewBunker);
	}
}

void AGS_SpaceInvaders24::ResetGame() {
	Level = 0;
	Lives = 3;
	Points = 0;
	GameState = EGameState::PLAYING;
	SpawnSwarm();
	SpawnPlayer();
	SpawnBunkers();
}

void AGS_SpaceInvaders24::BeginPlay() {
	Super::BeginPlay();

	GamePreviewActor = Cast<AGamePreviewActor>(UGameplayStatics::GetActorOfClass(this, AGamePreviewActor::StaticClass()));
	GamePreviewActor->ManualInitialize();

	ResetGame();
}

void AGS_SpaceInvaders24::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

const TArray<AEnemy *> &AGS_SpaceInvaders24::GetEnemies() const { return Enemies; }

#pragma region // Wrapped functions from GamePreviewActor
FIntPoint AGS_SpaceInvaders24::WorldToTexelPos(FVector WorldPos) const { return GamePreviewActor->WorldToTexelPos(WorldPos); }

FVector AGS_SpaceInvaders24::TexelToWorldPos(FIntPoint TexelPos) const { return GamePreviewActor->TexelToWorldPos(TexelPos); }

FVector AGS_SpaceInvaders24::GetForward() const { return GamePreviewActor->GetForward(); }

FVector AGS_SpaceInvaders24::GetUp() const { return GamePreviewActor->GetUp(); }

FRotator AGS_SpaceInvaders24::GetGameObjectOrientation() const { return GamePreviewActor->GetGameObjectOrientation(); }
#pragma endregion