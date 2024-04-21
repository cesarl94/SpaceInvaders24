// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"


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

// Called when the game starts or when spawned
void AEnemy::BeginPlay() { Super::BeginPlay(); }

// This function will be triggered in BP
void AEnemy::Animate_Implementation(bool Forward, float Rate) const {}

// This function will be triggered in BP
void AEnemy::DieAnimation_Implementation(bool Forward, float Rate) const {}

// Called every frame
void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (UKismetSystemLibrary::GetFrameCount() % 100 == 0) {
		if (UKismetSystemLibrary::GetFrameCount() % 200 == 0) {
			Animate(true, 2);
		} else {

			Animate(false, 2);
		}
	}
}
