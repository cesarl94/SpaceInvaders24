// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Bunker.h"

#include "Components/SimpleVoxel.h"
#include "Math/IntPoint.h"
#include "Utils/MathUtils.h"

void ABunker::SetBrickInCoordinate(int32 RelativeX, int32 RelativeY, USimpleVoxel *Brick) {
	int32 Index1D = RelativeY * FMath::RoundToInt(ActorLocalBoundsFloat.Z) + RelativeX;
	_BricksGrid[Index1D] = Brick;
}

void ABunker::ManualInitialize() {
	GetComponents<USimpleVoxel>(Bricks);

	// We need to storage a reference of each brick in a 2D grid, but since Unreal doesn't allow 2D arrays, we create 1D indexed array
	// We set the size of the "Grid" of bricks, and fill it will nullptr
	_BricksGrid.Init(nullptr, FMath::RoundToInt(ActorLocalBoundsFloat.Z) * FMath::RoundToInt(ActorLocalBoundsFloat.W));


	for (int32 i = 0; i < Bricks.Num(); i++) {
		USimpleVoxel *Brick = Bricks[i];
		FVector BrickRelativePosition = Brick->GetRelativeLocation();
		FIntPoint BrickRelativeTexelPosition = Relative3DToRelativeTexelPos(BrickRelativePosition);
		SetBrickInCoordinate(BrickRelativeTexelPosition.X, BrickRelativeTexelPosition.Y, Brick);
	}
}

void ABunker::ManualReset() {
	for (int32 i = 0; i < Bricks.Num(); i++) {
		USimpleVoxel *Brick = Bricks[i];
		Brick->SetEnabled(true);
	}
}

USimpleVoxel *ABunker::GetBrickInCoordinate(int32 RelativeX, int32 RelativeY) const {
	int32 Index1D = RelativeY * FMath::RoundToInt(ActorLocalBoundsFloat.Z) + RelativeX;
	return _BricksGrid[Index1D];
}

const TArray<USimpleVoxel *> &ABunker::GetAllBricks() const { return Bricks; }
