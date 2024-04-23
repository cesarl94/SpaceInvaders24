// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SwarmMind.h"

#include "Actors/Enemy.h"
#include "Components/GameTimeManager.h"
#include "Core/GS_SpaceInvaders24.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Utils/Enums.h"


USwarmMind::USwarmMind() { PrimaryComponentTick.bCanEverTick = false; }

FIntPoint USwarmMind::GetNextEnemyGridIDToUpdate() const {
	FIntPoint RetornedValue;

	if (LastUpdatedEnemyGridID.X == EnemiesPerRow - 1) {
		RetornedValue.X = 0;
		if (LastUpdatedEnemyGridID.Y == 0) {
			RetornedValue.Y = EnemyTypesByRow.Num() - 1;
		} else {
			RetornedValue.Y = LastUpdatedEnemyGridID.Y - 1;
		}
	} else {
		RetornedValue.X = LastUpdatedEnemyGridID.X + 1;
		RetornedValue.Y = LastUpdatedEnemyGridID.Y;
	}

	return RetornedValue;
}

AEnemy *USwarmMind::GetNextEnemyToUpdate() const { return nullptr; }

void USwarmMind::SetEnemyInIndexed2DArray(int32 X, int32 Y, class AEnemy *Enemy) {
	int32 Index1D = Y * EnemiesPerRow + X;
	_Enemies2D[Index1D] = Enemy;
}

void USwarmMind::OnNewGameState(EGameState NewGameState) {
	switch (NewGameState) {
	case EGameState::IN_MENU:
	case EGameState::READY_SET_GO:
	case EGameState::DYING:
	case EGameState::GAME_OVER:
		break;
	case EGameState::PLAYING_FORWARD:
	case EGameState::PLAYING_SLOW_TIME_DOWN:
	case EGameState::PLAYING_PAUSED_TIME:

		break;
	case EGameState::PLAYING_REVERSE:
		break;
	}
}

void USwarmMind::ManualInitialize() {
	AGS_SpaceInvaders24 *GameState = GetOwner<AGS_SpaceInvaders24>();

	_Enemies2D.SetNumUninitialized(EnemiesPerRow * EnemyTypesByRow.Num());

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator GameObjectOrientation = GameState->GetGameObjectOrientation();

	FIntPoint PositionDelta = TexelCoordOfTopLeftEnemyInFirstLevel;

	for (int32 i = 0; i < EnemyTypesByRow.Num(); i++) {
		for (int32 j = 0; j < EnemiesPerRow; j++) {
			TSubclassOf<AEnemy> *EnemySubclass = EnemyClasses.Find(EnemyTypesByRow[i]);
			if (EnemySubclass == nullptr) {
				continue;
			}
			FVector EnemyWorldPosition = GameState->TexelToWorldPos(PositionDelta);
			AEnemy *Enemy = GetWorld()->SpawnActor<AEnemy>(*EnemySubclass, EnemyWorldPosition, GameObjectOrientation, ActorSpawnParams);
			Enemy->ManualInitialize(FIntPoint(j, i));
			SetEnemyInIndexed2DArray(j, i, Enemy);
			Enemies.Add(Enemy);

			PositionDelta.X += SeparationBetweenEnemies.X;
		}
		PositionDelta.X -= SeparationBetweenEnemies.X * EnemiesPerRow;
		PositionDelta.Y += SeparationBetweenEnemies.Y;
	}
}

void USwarmMind::ManualReset(int32 Level) {
	AGS_SpaceInvaders24 *GameState = GetOwner<AGS_SpaceInvaders24>();

	MovingToRight = true;
	DirectionBlocked = true;
	OnBackwards = true;

	AccumulatedDeltaTime = 0;

	LastUpdatedEnemyGridID = FIntPoint(EnemiesPerRow - 1, 0);

	FIntPoint PositionDelta = TexelCoordOfTopLeftEnemyInFirstLevel + Level * (SeparationBetweenEnemies.Y / 2);
	if (PositionDelta.Y > TexelCoordOfTopLeftEnemyInLastLevel.Y) {
		PositionDelta.Y = TexelCoordOfTopLeftEnemyInLastLevel.Y;
	}

	for (int32 i = 0; i < EnemyTypesByRow.Num(); i++) {
		for (int32 j = 0; j < EnemiesPerRow; j++) {
			AEnemy *EnemyInThisPosition = GetEnemyInIndexed2DArray(j, i);
			EnemyInThisPosition->ManualReset(PositionDelta);
			PositionDelta.X += SeparationBetweenEnemies.X;
		}
		PositionDelta.X -= SeparationBetweenEnemies.X * EnemiesPerRow;
		PositionDelta.Y += SeparationBetweenEnemies.Y;
	}
}

FIntPoint USwarmMind::GetEnemyArraySize() const { return FIntPoint(EnemiesPerRow, EnemyTypesByRow.Num()); }

const TArray<AEnemy *> &USwarmMind::GetEnemies() const { return Enemies; }

TArray<AEnemy *> USwarmMind::GetLiveEnemies() const {
	TArray<AEnemy *> ReturnValue;

	for (AEnemy *Enemy : Enemies) {
		if (Enemy->IsAlive()) {
			ReturnValue.Add(Enemy);
		}
	}

	return ReturnValue;
}

AEnemy *USwarmMind::GetEnemyInIndexed2DArray(int32 X, int32 Y) const {
	int32 Index1D = Y * EnemiesPerRow + X;
	return _Enemies2D[Index1D];
}

void USwarmMind::ManualTick(float CrystalDeltaTime) {
	AGS_SpaceInvaders24 *GameState = GetOwner<AGS_SpaceInvaders24>();

	switch (GameState->GetGameState()) {
	case EGameState::IN_MENU:
	case EGameState::READY_SET_GO:
	case EGameState::DYING:
	case EGameState::GAME_OVER:
		break;
	case EGameState::PLAYING_FORWARD:
	case EGameState::PLAYING_SLOW_TIME_DOWN:
	case EGameState::PLAYING_PAUSED_TIME:
		{
			AccumulatedDeltaTime += CrystalDeltaTime;

			float IdealFrameDelta = 1.f / 60.f;

			while (AccumulatedDeltaTime >= IdealFrameDelta) {
				AccumulatedDeltaTime -= IdealFrameDelta;
			}
		}


		break;
	case EGameState::PLAYING_REVERSE:
		break;
	}
}