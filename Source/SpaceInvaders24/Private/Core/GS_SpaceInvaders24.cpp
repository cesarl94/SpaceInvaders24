#include "Core/GS_SpaceInvaders24.h"

#include "Actors/Bunker.h"
#include "Actors/Enemy.h"
#include "Actors/GamePreviewActor.h"
#include "Actors/LaserTank.h"
#include "Components/GameTimeManager.h"
#include "Components/SwarmMind.h"
#include "Core/PC_SpaceInvaders24.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Structs/EnemyRow.h"


AGS_SpaceInvaders24::AGS_SpaceInvaders24() {
	PrimaryActorTick.bCanEverTick = true;

	GameTimeManager = CreateDefaultSubobject<UGameTimeManager>(TEXT("Game Time Manager"));
	SwarmMind = CreateDefaultSubobject<USwarmMind>(TEXT("Swarm Mind"));
}

void AGS_SpaceInvaders24::SpawnSwarm() {
	_Enemies2D.SetNumUninitialized(EnemiesPerRow * EnemyDispositions.Num());

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator GameObjectOrientation = GetGameObjectOrientation();

	FIntPoint PositionDelta = TexelCoordOfTopLeftEnemyInFirstLevel + Level * 8;
	if (PositionDelta.Y > TexelCoordOfTopLeftEnemyInLastLevel.Y) {
		PositionDelta.Y = TexelCoordOfTopLeftEnemyInLastLevel.Y;
	}


	for (int32 i = 0; i < EnemyDispositions.Num(); i++) {
		FEnemyRow EnemyRowData = EnemyDispositions[i];
		for (int32 j = 0; j < EnemiesPerRow; j++) {
			TSubclassOf<AEnemy> *EnemySubclass = EnemyClasses.Find(EnemyRowData.EnemyType);
			if (EnemySubclass != nullptr) {
				FVector EnemyWorldPosition = TexelToWorldPos(PositionDelta);
				AEnemy *Enemy = GetWorld()->SpawnActor<AEnemy>(*EnemySubclass, EnemyWorldPosition, GameObjectOrientation, ActorSpawnParams);
				Enemy.ManualInitialize(FIntPoint(j, i));
				SetEnemyInIndexed2DArray(j, i, Enemy);
				Enemies.Add(Enemy);

				PositionDelta.X += SeparationBetweenEnemies.X;
			}
		}
		PositionDelta.X -= SeparationBetweenEnemies.X * EnemiesPerRow;
		PositionDelta.Y += SeparationBetweenEnemies.Y;
	}
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

	Player->StartGame();
}

void AGS_SpaceInvaders24::SetEnemyInIndexed2DArray(int32 X, int32 Y, class AEnemy *Enemy) {
	int32 Index1D = Y * EnemiesPerRow + X;
	_Enemies2D[Index1D] = Enemy;
}

void AGS_SpaceInvaders24::BeginPlay() {
	Super::BeginPlay();

	GamePreviewActor = Cast<AGamePreviewActor>(UGameplayStatics::GetActorOfClass(this, AGamePreviewActor::StaticClass()));
	GamePreviewActor->ManualInitialize();

	ResetGame();
}

void AGS_SpaceInvaders24::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	GameTimeManager->ManualTick(DeltaTime);

	SwarmMind->ManualTick(DeltaTime);
}

const TArray<AEnemy *> &AGS_SpaceInvaders24::GetEnemies() const { return Enemies; }

void AGS_SpaceInvaders24::OnPlayerControllerConnected(APlayerController *PC) {
	if (Player != nullptr && PC->GetPawn() == nullptr) {
		PC->Possess(Player);
	}
}

AEnemy *AGS_SpaceInvaders24::GetEnemyInIndexed2DArray(int32 X, int32 Y) const {
	int32 Index1D = Y * EnemiesPerRow + X;
	return _Enemies2D[Index1D];
}

#pragma region // Wrapped functions from GamePreviewActor
FIntPoint AGS_SpaceInvaders24::GetMapSize() const { return GamePreviewActor->GetMapSize(); }

FIntPoint AGS_SpaceInvaders24::WorldToTexelPos(FVector WorldPos) const { return GamePreviewActor->WorldToTexelPos(WorldPos); }

FVector AGS_SpaceInvaders24::TexelToWorldPos(FIntPoint TexelPos) const { return GamePreviewActor->TexelToWorldPos(TexelPos); }

FVector AGS_SpaceInvaders24::GetForward() const { return GamePreviewActor->GetForward(); }

FVector AGS_SpaceInvaders24::GetUp() const { return GamePreviewActor->GetUp(); }

FRotator AGS_SpaceInvaders24::GetGameObjectOrientation() const { return GamePreviewActor->GetGameObjectOrientation(); }
#pragma endregion