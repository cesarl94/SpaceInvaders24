// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Bunker.h"

#include "Components/SimpleVoxel.h"
#include "Math/IntPoint.h"
#include "Utils/MathUtils.h"

void ABunker::SetBrickInCoordinate(int32 RelativeX, int32 RelativeY, USimpleVoxel *Brick) {
	int32 Index1D = RelativeY * ActorLocalBounds.Z + RelativeX;
	_BricksGrid[Index1D] = Brick;
}

FIntPoint ABunker::Relative3DToRelativeTexelPos(FVector Relative3DPos) const {
	float RelativeTexelXAprox = UMathUtils::RuleOfFive(MinRelativeLocation.Y, 0, MaxRelativeLocation.Y, ActorLocalBounds.Z - 1, Relative3DPos.Y, false);
	float RelativeTexelYAprox = UMathUtils::RuleOfFive(MinRelativeLocation.Z, 0, MaxRelativeLocation.Z, ActorLocalBounds.W - 1, Relative3DPos.Z, false);
	return FIntPoint(FMath::RoundToInt(RelativeTexelXAprox), FMath::RoundToInt(RelativeTexelYAprox));
}

FVector ABunker::RelativeTexelToRelative3DPos(FIntPoint RelativeTexelPos) const {
	float Relative3DPosY = UMathUtils::RuleOfFive(0, MinRelativeLocation.Y, ActorLocalBounds.Z - 1, MaxRelativeLocation.Y, RelativeTexelPos.X, false);
	float Relative3DPosZ = UMathUtils::RuleOfFive(0, MinRelativeLocation.Z, ActorLocalBounds.W - 1, MaxRelativeLocation.Z, RelativeTexelPos.Y, false);
	return FVector(0, Relative3DPosY, Relative3DPosZ);
}


void ABunker::ManualInitialize() {
	MinRelativeLocation = SceneInMinRelativePos->GetRelativeLocation();
	MaxRelativeLocation = SceneInMaxRelativePos->GetRelativeLocation();

	GetComponents<USimpleVoxel>(Bricks);

	// We need to storage a reference of each brick in a 2D grid, but since Unreal doesn't allow 2D arrays, we create 1D indexed array
	// We set the size of the "Grid" of bricks, and fill it will nullptr
	_BricksGrid.SetNumUninitialized(ActorLocalBounds.Z * ActorLocalBounds.W);

	for (int32 i = 0; i < Bricks.Num(); i++) {
		USimpleVoxel *Brick = Bricks[i];
		FVector BrickRelativePosition = Brick->GetRelativeLocation();
		FIntPoint BrickRelativeTexelPosition = Relative3DToRelativeTexelPos(BrickRelativePosition);
		SetBrickInCoordinate(BrickRelativeTexelPosition.X, BrickRelativeTexelPosition.Y, Brick);
	}
}

const USimpleVoxel *ABunker::GetBrickInCoordinate(int32 RelativeX, int32 RelativeY) const {
	int32 Index1D = RelativeY * ActorLocalBounds.Z + RelativeX;
	return _BricksGrid[Index1D];
}

const TArray<USimpleVoxel *> &ABunker::GetAllBricks() const { return Bricks; }
