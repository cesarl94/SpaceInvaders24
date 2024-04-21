// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Bunker.h"

#include "Components/BunkerBrick.h"
#include "Utils/MathUtils.h"


ABunker::ABunker() {
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SceneComponent->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Collider->SetupAttachment(SceneComponent);

	BricksScene = CreateDefaultSubobject<USceneComponent>(TEXT("Bricks Scene"));
	BricksScene->SetupAttachment(SceneComponent);

	SceneInMinRelativePos = CreateDefaultSubobject<USceneComponent>(TEXT("Min Relative Pos Scene"));
	SceneInMinRelativePos->SetupAttachment(SceneComponent);

	SceneInMaxRelativePos = CreateDefaultSubobject<USceneComponent>(TEXT("Max Relative Pos Scene"));
	SceneInMaxRelativePos->SetupAttachment(SceneComponent);
}

void ABunker::SetBrickInCoordinate(int32 RelativeX, int32 RelativeY, UBunkerBrick *Brick) {
	int32 Index1D = RelativeY * BunkerSize.X + RelativeX;
	_BricksGrid[Index1D] = Brick;
}

FIntPoint ABunker::Relative3DToRelativeTexelPos(FVector Relative3DPos) const {
	float RelativeTexelXAprox = UMathUtils::RuleOfFive(MinRelativeLocation.Y, 0, MaxRelativeLocation.Y, BunkerSize.X - 1, Relative3DPos.Y, false);
	float RelativeTexelYAprox = UMathUtils::RuleOfFive(MinRelativeLocation.Z, 0, MaxRelativeLocation.Z, BunkerSize.Y - 1, Relative3DPos.Z, false);
	return FIntPoint(FMath::RoundToInt(RelativeTexelXAprox), FMath::RoundToInt(RelativeTexelYAprox));
}

FVector ABunker::RelativeTexelToRelative3DPos(FIntPoint RelativeTexelPos) const {
	float Relative3DPosY = UMathUtils::RuleOfFive(0, MinRelativeLocation.Y, BunkerSize.X - 1, MaxRelativeLocation.Y, RelativeTexelPos.X, false);
	float Relative3DPosZ = UMathUtils::RuleOfFive(0, MinRelativeLocation.Z, BunkerSize.Y - 1, MaxRelativeLocation.Z, RelativeTexelPos.Y, false);
	return FVector(0, Relative3DPosY, Relative3DPosZ);
}


void ABunker::ManualInitialize() {
	MinRelativeLocation = SceneInMinRelativePos->GetRelativeLocation();
	MaxRelativeLocation = SceneInMaxRelativePos->GetRelativeLocation();

	GetComponents<UBunkerBrick>(Bricks);

	// We need to storage a reference of each brick in a 2D grid, but since Unreal doesn't allow 2D arrays, we create 1D indexed array
	// We set the size of the "Grid" of bricks, and fill it will nullptr
	_BricksGrid.SetNumUninitialized(BunkerSize.X * BunkerSize.Y);

	for (int32 i = 0; i < Bricks.Num(); i++) {
		UBunkerBrick *Brick = Bricks[i];
		FVector BrickRelativePosition = Brick->GetRelativeLocation();
		FIntPoint BrickRelativeTexelPosition = Relative3DToRelativeTexelPos(BrickRelativePosition);
		SetBrickInCoordinate(BrickRelativeTexelPosition.X, BrickRelativeTexelPosition.Y, Brick);
	}
}

const UBunkerBrick *ABunker::GetBrickInCoordinate(int32 RelativeX, int32 RelativeY) const {
	int32 Index1D = RelativeY * BunkerSize.X + RelativeX;
	return _BricksGrid[Index1D];
}

const TArray<UBunkerBrick *> &ABunker::GetAllBricks() const { return Bricks; }
