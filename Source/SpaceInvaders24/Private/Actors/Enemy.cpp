// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy.h"

#include "Actors/BlastTrail.h"
#include "Actors/Crystal.h"
#include "Components/BoxComponent.h"
#include "Core/GS_SpaceInvaders24.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/IntPoint.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "Structs/BlastTrailData.h"


void AEnemy::SpawnBlastTrail() {
	if (BlastTrailData.BlastTrailActorClass == nullptr) {
		return;
	}

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));

	FIntPoint TexelIntPoint = GetIntTexelPosition() + BlastTrailData.Offset;
	FVector BlastTrailWorldPosition = GameState->TexelToWorldPos(TexelIntPoint);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABlastTrail *BlastTrail = GetWorld()->SpawnActor<ABlastTrail>(BlastTrailData.BlastTrailActorClass, BlastTrailWorldPosition, GameState->GetGameObjectOrientation(), ActorSpawnParams);
	BlastTrail->ManualInitialize(BlastTrailData.Duration);
}

void AEnemy::SpawnCrystal() {
	if (CrystalDropData.CrystalDroppedClass == nullptr) {
		return;
	}

	int32 RandomNum = FMath::RandRange(1, 100);
	if (RandomNum > CrystalDropData.CrystalDropProbability) {
		return;
	}

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));

	FIntPoint TexelIntPoint = GetIntTexelPosition() + CrystalDropData.CrystalOffset;
	FVector CrystalWorldPosition = GameState->TexelToWorldPos(TexelIntPoint);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACrystal *Crystal = GetWorld()->SpawnActor<ACrystal>(CrystalDropData.CrystalDroppedClass, CrystalWorldPosition, GameState->GetGameObjectOrientation(), ActorSpawnParams);
	Crystal->ManualInitialize();
}

// This function will be triggered in BP
void AEnemy::Animate_Implementation(bool Forward, float Rate) const {}

void AEnemy::ManualInitialize(FIntPoint CoordinateInGrid) {
	EnemyCoordinateInGrid = CoordinateInGrid;
	GraphicNodes->SetVisibility(false, true);
}

void AEnemy::TriggerMoveAnimation(float PlayRate) {
	Animate(!LastAnimationWasForward, 2 * PlayRate);
	LastAnimationWasForward = !LastAnimationWasForward;
}

void AEnemy::ManualReset(FIntPoint NewTexelPosition) {
	SetTexelPosition(FVector2D(NewTexelPosition.X, NewTexelPosition.Y));
	LastAnimationWasForward = false;
	// we need to set to the first frame inmediatly
	Animate(false, 1000000.f);

	GraphicNodes->SetVisibility(true, true);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Alive = true;
}

FIntPoint AEnemy::GetCoordinateInEnemyGrid() const { return EnemyCoordinateInGrid; }

bool AEnemy::IsAlive() const { return Alive; }

void AEnemy::Kill(bool IsForcedKill) {
	Alive = false;
	GraphicNodes->SetVisibility(false, true);

	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	int32 PointsGiven = 0;

	if (!IsForcedKill) {
		SpawnBlastTrail();
		SpawnCrystal();

		int32 PointsGivenRandomId = FMath::RandRange(0, PointsThatCouldGive.Num() - 1);
		PointsGiven = PointsThatCouldGive[PointsGivenRandomId];
	}

	OnDie.Broadcast(this, PointsGiven);
}