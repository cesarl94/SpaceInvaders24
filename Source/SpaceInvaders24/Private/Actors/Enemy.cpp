// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/IntPoint.h"
#include "Math/Vector2D.h"


// Sets default values
AEnemy::AEnemy() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SceneComponent->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Collider->SetupAttachment(SceneComponent);

	GraphicNodes = CreateDefaultSubobject<USceneComponent>(TEXT("Graphic Nodes"));
	GraphicNodes->SetupAttachment(SceneComponent);
}

void AEnemy::ManualInitialize(FIntPoint CoordinateInGrid) { EnemyCoordinateInGrid = CoordinateInGrid; }

void AEnemy::ManualReset(FIntPoint NewTexelPosition) {
	SetTexelPosition(FVector2D(NewTexelPosition.X, NewTexelPosition.Y));
	Alive = true;
}

FIntPoint AEnemy::GetEnemyCoordinateInGrid() const { return EnemyCoordinateInGrid; }

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

void AEnemy::Die() {
	Alive = false;
	OnDie.Broadcast(this);
}