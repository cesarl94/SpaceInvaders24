// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy.h"

#include "Actors/BlastTrail.h"
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

AEnemy::AEnemy() {
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SceneComponent->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Collider->SetupAttachment(SceneComponent);

	GraphicNodes = CreateDefaultSubobject<USceneComponent>(TEXT("Graphic Nodes"));
	GraphicNodes->SetupAttachment(SceneComponent);
}

void AEnemy::ManualInitialize(FIntPoint CoordinateInGrid) {
	EnemyCoordinateInGrid = CoordinateInGrid;
	GraphicNodes->SetVisibility(false, true);
}

void AEnemy::ManualReset(FIntPoint NewTexelPosition) {
	SetTexelPosition(FVector2D(NewTexelPosition.X, NewTexelPosition.Y));
	GraphicNodes->SetVisibility(true, true);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Alive = true;
}

FIntPoint AEnemy::GetCoordinateInEnemyGrid() const { return EnemyCoordinateInGrid; }

// This function will be triggered in BP
void AEnemy::Animate_Implementation(bool Forward, float Rate) const {}

// This function will be triggered in BP
void AEnemy::DieAnimation_Implementation(bool Forward, float Rate) const {}

// Called every frame
void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// TODO: replace this hard-coded reference
	if (UKismetSystemLibrary::GetFrameCount() % 50 == 0) {
		if (UKismetSystemLibrary::GetFrameCount() % 100 == 0) {
			Animate(true, 2);
		} else {
			Animate(false, 2);
		}
	}
}

bool AEnemy::IsAlive() const { return Alive; }

void AEnemy::Kill(bool IsForcedKill) {
	Alive = false;
	GraphicNodes->SetVisibility(false, true);

	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!IsForcedKill) {
		SpawnBlastTrail();
	}

	int32 PointsGivenRandomId = FMath::RandRange(0, PointsThatCouldGive.Num() - 1);
	int32 PointsGiven = IsForcedKill ? 0 : PointsThatCouldGive[PointsGivenRandomId];

	OnDie.Broadcast(this, PointsGiven);
}