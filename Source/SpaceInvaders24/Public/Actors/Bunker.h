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
	// CAUTION: Ultra-private variable
	// Since Unreal doesn't allow 2-dimensional-arrays, we'll use a 1D array indexed :(
	// Use with the functions: "GetBrickInCoordinate" and "SetBrickInCoordinate"
	// CAUTION2: Double reference array togheter with the another array of bricks, "TArray<USimpleVoxel *> Bricks".
	UPROPERTY()
	TArray<USimpleVoxel *> _BricksGrid;

	// TODO: Comentar
	UPROPERTY()
	TArray<USimpleVoxel *> Bricks;

	// TODO: Comentar esto
	UPROPERTY()
	FVector MinRelativeLocation;

	// TODO: Comentar esto
	UPROPERTY()
	FVector MaxRelativeLocation;

	// TODO: comentar que hace esto
	UFUNCTION()
	void SetBrickInCoordinate(int32 RelativeX, int32 RelativeY, USimpleVoxel *Brick);

	// TODO: comentar que hace esto
	UFUNCTION()
	FIntPoint Relative3DToRelativeTexelPos(FVector Relative3DPos) const;

	// TODO: comentar que hace esto
	UFUNCTION()
	FVector RelativeTexelToRelative3DPos(FIntPoint RelativeTexelPos) const;

public:
	// Called from GameState's BeginPlay
	void ManualInitialize();

	// TODO: comentar que hace esto
	UFUNCTION()
	const USimpleVoxel *GetBrickInCoordinate(int32 RelativeX, int32 RelativeY) const;

	// Returns all the bricks. CAUTION: Enabled and disabled bricks.
	UFUNCTION()
	const TArray<USimpleVoxel *> &GetAllBricks() const;
};
