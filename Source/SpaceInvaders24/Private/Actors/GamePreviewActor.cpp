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
	PrimaryActorTick.bCanEverTick = true;

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

FIntPoint AGamePreviewActor::WorldToTexelPos(FVector WorldPos) {
	FVector RelativePosition = GetActorTransform().InverseTransformPosition(WorldPos);
	float TexelXAprox = UMathUtils::RuleOfFive(MinRelativeLocation.X, 0, MaxRelativeLocation.X, MapSize.X, RelativePosition.X, false);
	float TexelYAprox = UMathUtils::RuleOfFive(MinRelativeLocation.Y, 0, MaxRelativeLocation.Y, MapSize.Y, RelativePosition.Y, false);
	return FIntPoint(FMath::RoundToInt(TexelXAprox), FMath::RoundToInt(TexelYAprox));
}

FVector AGamePreviewActor::TexelToWorldPos(FIntPoint TexelPos) {
	float RelativeTexelX = UMathUtils::RuleOfFive(0, MinRelativeLocation.X, MapSize.X, MaxRelativeLocation.X, TexelPos.X, false);
	float RelativeTexelY = UMathUtils::RuleOfFive(0, MinRelativeLocation.Y, MapSize.Y, MaxRelativeLocation.Y, TexelPos.Y, false);
	return GetActorTransform().TransformPosition(FVector(RelativeTexelX, RelativeTexelY, 0));
}


FVector AGamePreviewActor::GetForward() { return RelativeForward->GetForwardVector(); }

FVector AGamePreviewActor::GetUp() { return RelativeUp->GetUpVector(); }

FRotator AGamePreviewActor::GetGameObjectOrientation() { return GameObjectOrientation; }