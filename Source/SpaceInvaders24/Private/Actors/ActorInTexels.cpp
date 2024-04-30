// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ActorInTexels.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Core/GS_SpaceInvaders24.h"
#include "Kismet/GameplayStatics.h"
#include "Math/IntVector.h"
#include "Math/Vector2D.h"
#include "Math/Vector4.h"
#include "Utils/Enums.h"
#include "Utils/MathUtils.h"


void AActorInTexels::BeginPlay() {
	Super::BeginPlay();

	MinRelativeLocation = SceneInMinRelativePos->GetRelativeLocation();
	MaxRelativeLocation = SceneInMaxRelativePos->GetRelativeLocation();

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));
	CurrentTexelPosition = GameState->WorldToTexelPos(RootComponent->GetComponentLocation());
	TexelVelocity = FVector2D(0, 0);
}

FIntPoint AActorInTexels::Relative3DToRelativeTexelPos(FVector Relative3DPos) const {
	float RelativeTexelXAprox = UMathUtils::RuleOfFive(MinRelativeLocation.Y, 0, MaxRelativeLocation.Y, FMath::RoundToInt(ActorLocalBoundsFloat.Z) - 1, Relative3DPos.Y, false);
	float RelativeTexelYAprox = UMathUtils::RuleOfFive(MinRelativeLocation.Z, 0, MaxRelativeLocation.Z, FMath::RoundToInt(ActorLocalBoundsFloat.W) - 1, Relative3DPos.Z, false);
	return FIntPoint(FMath::RoundToInt(RelativeTexelXAprox), FMath::RoundToInt(RelativeTexelYAprox));
}

FVector AActorInTexels::RelativeTexelToRelative3DPos(FIntPoint RelativeTexelPos) const {
	float Relative3DPosY = UMathUtils::RuleOfFive(0, MinRelativeLocation.Y, FMath::RoundToInt(ActorLocalBoundsFloat.Z) - 1, MaxRelativeLocation.Y, RelativeTexelPos.X, false);
	float Relative3DPosZ = UMathUtils::RuleOfFive(0, MinRelativeLocation.Z, FMath::RoundToInt(ActorLocalBoundsFloat.W) - 1, MaxRelativeLocation.Z, RelativeTexelPos.Y, false);
	return FVector(0, Relative3DPosY, Relative3DPosZ);
}

AActorInTexels::AActorInTexels() {
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);


	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Collider->SetupAttachment(SceneComponent);

	GraphicNodes = CreateDefaultSubobject<USceneComponent>(TEXT("Graphic Nodes"));
	GraphicNodes->SetupAttachment(SceneComponent);

	SceneInMinRelativePos = CreateDefaultSubobject<USceneComponent>(TEXT("Min Relative Pos Scene"));
	SceneInMinRelativePos->SetupAttachment(SceneComponent);

	SceneInMaxRelativePos = CreateDefaultSubobject<USceneComponent>(TEXT("Max Relative Pos Scene"));
	SceneInMaxRelativePos->SetupAttachment(SceneComponent);
}

void AActorInTexels::SetVisibility(bool Visible) { GraphicNodes->SetVisibility(Visible, true); }

void AActorInTexels::ToggleVisibility() { SetVisibility(!GetVisibility()); }

bool AActorInTexels::GetVisibility() const { return GraphicNodes->GetVisibleFlag(); }

void AActorInTexels::ApplyVelocity(float DeltaTime) {
	FVector2D PrevTexelPosition = GetFloatTexelPosition();
	FVector2D NewTexelPosition = PrevTexelPosition + GetTexelVelocity() * DeltaTime;

	SetTexelPosition(NewTexelPosition, true);
}

FVector2D AActorInTexels::GetFloatTexelPosition() const { return CurrentTexelPosition; }

FIntPoint AActorInTexels::GetIntTexelPosition() const { return FIntPoint(FMath::RoundToInt(CurrentTexelPosition.X), FMath::RoundToInt(CurrentTexelPosition.Y)); }

void AActorInTexels::SetTexelPosition(FVector2D NewTexelPosition, bool Sweep) {
	FIntPoint PrevIntTexelPosition = GetIntTexelPosition();

	FIntVector4 PositionLimits = FIntVector4(ActorLimits.X - FMath::RoundToInt(ActorLocalBoundsFloat.X), ActorLimits.Y - FMath::RoundToInt(ActorLocalBoundsFloat.Y),
											 ActorLimits.X + ActorLimits.Z - FMath::RoundToInt(ActorLocalBoundsFloat.X) - FMath::RoundToInt(ActorLocalBoundsFloat.Z),
											 ActorLimits.Y + ActorLimits.W - FMath::RoundToInt(ActorLocalBoundsFloat.Y) - FMath::RoundToInt(ActorLocalBoundsFloat.W));

	if (ClampPosition) {
		NewTexelPosition.X = FMath::Clamp(NewTexelPosition.X, PositionLimits.X, PositionLimits.X + PositionLimits.Z);
		NewTexelPosition.Y = FMath::Clamp(NewTexelPosition.Y, PositionLimits.Y, PositionLimits.Y + PositionLimits.W);
	}

	CurrentTexelPosition = NewTexelPosition;


	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));
	FIntPoint NewIntTexelPosition = GetIntTexelPosition();

	FVector NewWorldPos = GameState->TexelToWorldPos(NewIntTexelPosition);
	RootComponent->SetWorldLocation(NewWorldPos, Sweep);


	if (NewIntTexelPosition.X <= PositionLimits.X && PrevIntTexelPosition.X > PositionLimits.X) {
		OnTouchLimit.Broadcast(EDirection::LEFT);
	}
	if (NewIntTexelPosition.Y <= PositionLimits.Y && PrevIntTexelPosition.Y > PositionLimits.Y) {
		OnTouchLimit.Broadcast(EDirection::UP);
	}
	if (NewIntTexelPosition.X >= PositionLimits.Z && PrevIntTexelPosition.X < PositionLimits.Z) {
		OnTouchLimit.Broadcast(EDirection::RIGHT);
	}
	if (NewIntTexelPosition.Y >= PositionLimits.W && PrevIntTexelPosition.Y < PositionLimits.W) {
		OnTouchLimit.Broadcast(EDirection::DOWN);
	}
}

FVector2D AActorInTexels::GetTexelVelocity() const { return TexelVelocity; }

void AActorInTexels::SetTexelVelocity(FVector2D NewTexelVelocity) { TexelVelocity = NewTexelVelocity; }

FIntVector4 AActorInTexels::GetIntTexelBoundingBox() const {
	FIntPoint IntTexelPosition = GetIntTexelPosition();

	return FIntVector4(IntTexelPosition.X + FMath::RoundToInt(ActorLocalBoundsFloat.X), IntTexelPosition.Y + FMath::RoundToInt(ActorLocalBoundsFloat.Y), FMath::RoundToInt(ActorLocalBoundsFloat.Z),
					   FMath::RoundToInt(ActorLocalBoundsFloat.W));
}