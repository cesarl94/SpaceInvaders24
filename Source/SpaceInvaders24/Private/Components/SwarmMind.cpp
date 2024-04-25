// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SwarmMind.h"

#include "Actors/Enemy.h"
#include "Components/GameTimeManager.h"
#include "Core/GS_SpaceInvaders24.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "Utils/Enums.h"


USwarmMind::USwarmMind() { PrimaryComponentTick.bCanEverTick = false; }

FIntPoint USwarmMind::GetNextEnemyGridIDToUpdate(FIntPoint LastGridID) const {
	FIntPoint RetornedValue;

	if (OnBackwards) {
		if (LastGridID.X == 0) {
			RetornedValue = EnemiesPerRow - 1;
			if (LastGridID.Y == EnemyTypesByRow.Num() - 1) {
				RetornedValue.Y = 0;
			} else {
				RetornedValue.Y = LastGridID.Y + 1;
			}
		} else {
			RetornedValue.X = LastGridID.X - 1;
			RetornedValue.Y = LastGridID.Y;
		}

	} else {
		if (LastGridID.X == EnemiesPerRow - 1) {
			RetornedValue.X = 0;
			if (LastGridID.Y == 0) {
				RetornedValue.Y = EnemyTypesByRow.Num() - 1;
			} else {
				RetornedValue.Y = LastGridID.Y - 1;
			}
		} else {
			RetornedValue.X = LastGridID.X + 1;
			RetornedValue.Y = LastGridID.Y;
		}
	}

	return RetornedValue;
}

FIntPoint USwarmMind::GetCoordinateOfLastAliveEnemyToUpdate() const {

	if (OnBackwards) {
		for (int32 x = 0; x < EnemiesPerRow; x++) {
			for (int32 y = EnemyTypesByRow.Num() - 1; y >= 0; y--) {
				AEnemy *EnemyInThatPos = GetEnemyInIndexed2DArray(x, y);
				if (EnemyInThatPos->IsAlive()) {
					return EnemyInThatPos->GetCoordinateInEnemyGrid();
				}
			}
		}

		return FIntPoint(-1, -1);
	}

	for (int32 x = EnemiesPerRow - 1; x >= 0; x--) {
		for (int32 y = 0; y < EnemyTypesByRow.Num(); y++) {
			AEnemy *EnemyInThatPos = GetEnemyInIndexed2DArray(x, y);
			if (EnemyInThatPos->IsAlive()) {
				return EnemyInThatPos->GetCoordinateInEnemyGrid();
			}
		}
	}

	return FIntPoint(-1, -1);
}

void USwarmMind::FixedUpdate() {
	if (GameOverWasDispatched) {
		return;
	}

	FIntPoint LastCoordinateToUpdate = GetCoordinateOfLastAliveEnemyToUpdate();
	if (LastCoordinateToUpdate.X == -1 && LastCoordinateToUpdate.Y == -1) {
		return;
	}

	bool AlreadyUpdatedLastOne = (LastUpdatedEnemyGridID.X == LastCoordinateToUpdate.X && LastUpdatedEnemyGridID.Y == LastCoordinateToUpdate.Y);
	bool CanMove = true;
	if (AlreadyUpdatedLastOne) {
		if (!DirectionBlocked) {
			// if we are on backwards we go Upper, so, always we can
			bool CanGoLower = OnBackwards || CanEnemiesGoLower();

			if (CanGoLower) {
				MovingToRight = !MovingToRight;
				MovingDown = true;
				DirectionBlocked = true;
			} else {
				CanMove = false;
				GameOverWasDispatched = true;
				EnemiesCantGoLower.Broadcast();
			}
		} else {
			MovingDown = false;
		}
	}

	if (CanMove) {
		AEnemy *NextEnemyToUpdate = GetNextEnemyToUpdate();
		if (NextEnemyToUpdate == nullptr) {
			return;
		}

		// Since MovingToRight is when time goes forward, when we're OnBackwards, that "right" means left, so, both variables cancel each other
		float HorizontalMovement = (MovingToRight ? 1 : -1) * (OnBackwards ? -1 : 1) * SwarmVelocity;
		// The same with the vertical movement
		float VerticalMovement = (MovingDown ? SeparationBetweenEnemies.Y / 2 : 0) * (OnBackwards ? -1 : 1);

		FVector2D EnemyTexelPosition = NextEnemyToUpdate->GetFloatTexelPosition();
		EnemyTexelPosition += FVector2D(HorizontalMovement, VerticalMovement);
		NextEnemyToUpdate->SetTexelPosition(EnemyTexelPosition, !MovingDown);

		NextEnemyToUpdate->TriggerMoveAnimation(static_cast<float>(EnemiesPerRow * EnemyTypesByRow.Num()) / static_cast<float>(AliveEnemiesCount));

		LastUpdatedEnemyGridID = NextEnemyToUpdate->GetCoordinateInEnemyGrid();
	}
}

AEnemy *USwarmMind::GetNextEnemyToUpdate() const {
	int32 MaxTries = EnemiesPerRow * EnemyTypesByRow.Num();

	FIntPoint NextEnemyGridId = LastUpdatedEnemyGridID;
	do {
		NextEnemyGridId = GetNextEnemyGridIDToUpdate(NextEnemyGridId);
		AEnemy *NextEnemy = GetEnemyInIndexed2DArray(NextEnemyGridId.X, NextEnemyGridId.Y);

		if (NextEnemy->IsAlive()) {
			return NextEnemy;
		}

	} while (--MaxTries);

	return nullptr;
}

void USwarmMind::SetEnemyInIndexed2DArray(int32 X, int32 Y, class AEnemy *Enemy) {
	int32 Index1D = Y * EnemiesPerRow + X;
	_Enemies2D[Index1D] = Enemy;
}

void USwarmMind::OnEnemyTouchBorder(EDirection Direction) {
	if ((Direction == EDirection::LEFT || Direction == EDirection::RIGHT) && !MovingDown) {
		DirectionBlocked = false;
	}
}

void USwarmMind::OnEnemyDied(class AEnemy *EnemyDied, int32 Points) {

	AliveEnemiesCount--;

	EnemyDiedEvent.Broadcast(EnemyDied, Points);
	if (AliveEnemiesCount == 0) {
		KilledAllEnemies.Broadcast();
	}
}

int32 USwarmMind::GetHighestTexelYPositionOfAliveEnemy() {
	TArray<AEnemy *> LiveEnemies = GetLiveEnemies();

	int32 HighestTexelYPosition = -1000000;

	for (AEnemy *Enemy : LiveEnemies) {
		int32 TexelYPosition = Enemy->GetIntTexelPosition().Y;
		if (HighestTexelYPosition < TexelYPosition) {
			HighestTexelYPosition = TexelYPosition;
		}
	}

	return HighestTexelYPosition;
}

bool USwarmMind::CanEnemiesGoLower() {
	int32 BottomEnemyTexelYPosition = GetHighestTexelYPositionOfAliveEnemy();
	return BottomEnemyTexelYPosition < LastValidTexelCoordY;
}

void USwarmMind::OnNewGameState(EGameState NewGameState) {
	switch (NewGameState) {
	case EGameState::IN_MENU:
	case EGameState::READY_SET_GO:
	case EGameState::DYING:
	case EGameState::GAME_OVER:
		break;
	case EGameState::PASSING_LEVEL:
	case EGameState::PLAYING_FORWARD:
	case EGameState::PLAYING_SLOW_TIME_DOWN:
	case EGameState::PLAYING_PAUSED_TIME:
		if (OnBackwards) {

			LastUpdatedEnemyGridID = GetNextEnemyGridIDToUpdate(LastUpdatedEnemyGridID);
			OnBackwards = false;
		}
		break;
	case EGameState::PLAYING_REVERSE:
		if (!OnBackwards) {
			UKismetSystemLibrary::PrintString(GetWorld(), "PLAYING_REVERSE", true, true, FColor::Yellow, 5);
			LastUpdatedEnemyGridID = GetNextEnemyGridIDToUpdate(LastUpdatedEnemyGridID);
			OnBackwards = true;
		}
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
			Enemy->OnTouchLimit.AddUniqueDynamic(this, &USwarmMind::OnEnemyTouchBorder);
			Enemy->OnDie.AddUniqueDynamic(this, &USwarmMind::OnEnemyDied);

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
	OnBackwards = false;
	MovingDown = false;
	GameOverWasDispatched = false;

	AccumulatedDeltaTime = 0;
	AliveEnemiesCount = EnemiesPerRow * EnemyTypesByRow.Num();

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

void USwarmMind::ManualTick(float CrystalDeltaTime, float CrystalTotalSeconds) {
	AGS_SpaceInvaders24 *GameState = GetOwner<AGS_SpaceInvaders24>();

	// Very ugly FSM
	switch (GameState->GetGameState()) {
	case EGameState::IN_MENU:
	case EGameState::READY_SET_GO:
	case EGameState::DYING:
	case EGameState::GAME_OVER:
	case EGameState::PASSING_LEVEL:
		break;
	case EGameState::PLAYING_FORWARD:
	case EGameState::PLAYING_SLOW_TIME_DOWN:
	case EGameState::PLAYING_PAUSED_TIME:
	case EGameState::PLAYING_REVERSE:
		{
			AccumulatedDeltaTime += CrystalDeltaTime;

			float IdealFrameDelta = 1.f / 60.f;

			while (AccumulatedDeltaTime >= IdealFrameDelta) {
				AccumulatedDeltaTime -= IdealFrameDelta;
				FixedUpdate();
			}
		}

		break;
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
