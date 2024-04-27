// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GamePreviewActor.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
#include "Math/TransformNonVectorized.h"
#include "Math/Vector.h"
#include "Utils/MathUtils.h"


AGamePreviewActor::AGamePreviewActor() {
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SceneComponent->SetupAttachment(RootComponent);

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Plane->SetupAttachment(SceneComponent);

	RelativeForward = CreateDefaultSubobject<UArrowComponent>(TEXT("Relative Forward"));
	RelativeForward->SetupAttachment(SceneComponent);

	RelativeUp = CreateDefaultSubobject<UArrowComponent>(TEXT("Relative Up"));
	RelativeUp->SetupAttachment(SceneComponent);

	SceneInMinRelativePos = CreateDefaultSubobject<USceneComponent>(TEXT("Min Relative Pos Scene"));
	SceneInMinRelativePos->SetupAttachment(SceneComponent);

	SceneInMaxRelativePos = CreateDefaultSubobject<USceneComponent>(TEXT("Max Relative Pos Scene"));
	SceneInMaxRelativePos->SetupAttachment(SceneComponent);
}

void AGamePreviewActor::ManualInitialize() {
	MinRelativeLocation = SceneInMinRelativePos->GetRelativeLocation();
	MaxRelativeLocation = SceneInMaxRelativePos->GetRelativeLocation();

	FVector ForwardVector = RelativeForward->GetForwardVector();
	FVector UpVector = RelativeUp->GetUpVector();
	GameObjectOrientation = UKismetMathLibrary::MakeRotFromXZ(ForwardVector, UpVector);
}

FIntPoint AGamePreviewActor::GetMapSize() const { return MapSize; }

FIntPoint AGamePreviewActor::WorldToTexelPos(FVector WorldPos) const {
	FVector RelativePosition = GetActorTransform().InverseTransformPosition(WorldPos);
	float TexelXAprox = UMathUtils::RuleOfFive(MinRelativeLocation.X, 0, MaxRelativeLocation.X, MapSize.X - 1, RelativePosition.X, false);
	float TexelYAprox = UMathUtils::RuleOfFive(MinRelativeLocation.Y, 0, MaxRelativeLocation.Y, MapSize.Y - 1, RelativePosition.Y, false);
	// We subtract 0.01 to each number because we want to round to floor texels and half (E.g.  4 -> 3.99 = 4 or 4.5 -> 4.49 = 4)
	return FIntPoint(FMath::RoundToInt(TexelXAprox - 0.01f), FMath::RoundToInt(TexelYAprox - 0.01f));
}

FVector AGamePreviewActor::TexelToWorldPos(FIntPoint TexelPos) const {
	float RelativeTexelX = UMathUtils::RuleOfFive(0, MinRelativeLocation.X, MapSize.X - 1, MaxRelativeLocation.X, TexelPos.X, false);
	float RelativeTexelY = UMathUtils::RuleOfFive(0, MinRelativeLocation.Y, MapSize.Y - 1, MaxRelativeLocation.Y, TexelPos.Y, false);
	return GetActorTransform().TransformPosition(FVector(RelativeTexelX, RelativeTexelY, 0));
}

FVector AGamePreviewActor::GetForward() const { return RelativeForward->GetForwardVector(); }

FVector AGamePreviewActor::GetUp() const { return RelativeUp->GetUpVector(); }

FRotator AGamePreviewActor::GetGameObjectOrientation() const { return GameObjectOrientation; }