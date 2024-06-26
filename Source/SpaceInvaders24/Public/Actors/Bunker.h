// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/ActorInTexels.h"
#include "Components/SimpleVoxel.h"
#include "CoreMinimal.h"
#include "Math/IntPoint.h"

#include "Bunker.generated.h"


UCLASS()
class SPACEINVADERS24_API ABunker : public AActorInTexels {
	GENERATED_BODY()

private:
	/**
	 * 	CAUTION: Ultra-private variable. Do not touch
	 * Since Unreal doesn't allow 2-dimensional-arrays, we'll use a 1D array indexed :(
	 * Use with the functions: "GetBrickInCoordinate" and "SetBrickInCoordinate"
	 * CAUTION2: Double reference array togheter with the another array of bricks, "TArray<USimpleVoxel *> Bricks".
	 */
	UPROPERTY()
	TArray<USimpleVoxel *> _BricksGrid;

	/**
	 * Contains all the bricks.
	 * CAUTION: Double reference array togheter with the another array of bricks, "TArray<USimpleVoxel *> _BricksGrid".
	 */
	UPROPERTY()
	TArray<USimpleVoxel *> Bricks;

	// This function is for placing bricks in _BricksGrid array without directly accessing the variable.
	UFUNCTION()
	void SetBrickInCoordinate(int32 RelativeX, int32 RelativeY, USimpleVoxel *Brick);

public:
	// Called from GameState's BeginPlay after creation
	void ManualInitialize();

	void ManualReset();

	// This function is for get bricks from the _BricksGrid array without directly accessing the variable.
	UFUNCTION()
	USimpleVoxel *GetBrickInCoordinate(int32 RelativeX, int32 RelativeY) const;

	// Returns all the bricks. CAUTION: Enabled and disabled bricks.
	UFUNCTION()
	const TArray<USimpleVoxel *> &GetAllBricks() const;
};
