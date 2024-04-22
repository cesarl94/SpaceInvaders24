// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ActorInTexels.h"

#include "Core/GS_SpaceInvaders24.h"
#include "Kismet/GameplayStatics.h"
#include "Math/IntVector.h"
#include "Math/Vector2D.h"
#include "Utils/Enums.h"
#include "Utils/MathUtils.h"


void AActorInTexels::BeginPlay() {
	Super::BeginPlay();

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));
	CurrentTexelPosition = GameState->WorldToTexelPos(RootComponent->GetComponentLocation());
	TexelVelocity = FVector2D(0, 0);
}

void AActorInTexels::ApplyVelocity(float DeltaTime) {
	FVector2D PrevTexelPosition = GetFloatTexelPosition();
	FVector2D NewTexelPosition = PrevTexelPosition + GetTexelVelocity() * DeltaTime;

	SetTexelPosition(NewTexelPosition, true);
}

FVector2D AActorInTexels::GetFloatTexelPosition() { return CurrentTexelPosition; }

FIntPoint AActorInTexels::GetIntTexelPosition() { return FIntPoint(FMath::RoundToInt(CurrentTexelPosition.X), FMath::RoundToInt(CurrentTexelPosition.Y)); }

void AActorInTexels::SetTexelPosition(FVector2D NewTexelPosition, bool Sweep) {
	FIntPoint PrevIntTexelPosition = GetIntTexelPosition();

	FIntVector4 PositionLimits = FIntVector4(ActorLimits.X - ActorLocalBounds.X, ActorLimits.X - ActorLocalBounds.Y, ActorLimits.X + ActorLimits.Z - ActorLocalBounds.X - ActorLocalBounds.Z,
											 ActorLimits.Y + ActorLimits.W - ActorLocalBounds.Y - ActorLocalBounds.W);

	NewTexelPosition.X = FMath::Clamp(NewTexelPosition.X, PositionLimits.X, PositionLimits.X + PositionLimits.Z);
	NewTexelPosition.Y = FMath::Clamp(NewTexelPosition.Y, PositionLimits.Y, PositionLimits.Y + PositionLimits.W);

	CurrentTexelPosition = NewTexelPosition;


	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));
	FIntPoint NewIntTexelPosition = GetIntTexelPosition();

	FVector NewWorldPos = GameState->TexelToWorldPos(NewIntTexelPosition);
	RootComponent->SetWorldLocation(NewWorldPos, Sweep);


	if (NewIntTexelPosition.X <= PositionLimits.X && PrevIntTexelPosition.X != PositionLimits.X) {
		OnTouchLimit.Broadcast(EDirection::LEFT);
	}
	if (NewIntTexelPosition.Y <= PositionLimits.Y && PrevIntTexelPosition.Y != PositionLimits.Y) {
		OnTouchLimit.Broadcast(EDirection::UP);
	}
	if (NewIntTexelPosition.X >= PositionLimits.X + PositionLimits.Z && PrevIntTexelPosition.X != PositionLimits.X + PositionLimits.Z) {
		OnTouchLimit.Broadcast(EDirection::RIGHT);
	}
	if (NewIntTexelPosition.Y <= PositionLimits.Y + PositionLimits.W && PrevIntTexelPosition.Y != PositionLimits.Y + PositionLimits.W) {
		OnTouchLimit.Broadcast(EDirection::DOWN);
	}
}

FVector2D AActorInTexels::GetTexelVelocity() { return TexelVelocity; }

void AActorInTexels::SetTexelVelocity(FVector2D NewTexelVelocity) { TexelVelocity = NewTexelVelocity; }

FIntVector4 AActorInTexels::GetIntTexelBoundingBox() {
	FIntPoint IntTexelPosition = GetIntTexelPosition();

	return FIntVector4(IntTexelPosition.X + ActorLocalBounds.X, IntTexelPosition.Y + ActorLocalBounds.Y, ActorLocalBounds.Z, ActorLocalBounds.W);
}