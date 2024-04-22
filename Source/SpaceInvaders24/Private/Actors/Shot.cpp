// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Shot.h"

#include "Components/BoxComponent.h"
#include "Core/GS_SpaceInvaders24.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void AShot::BeginPlay() { Super::BeginPlay(); }

AShot::AShot() {
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SceneComponent->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Collider->SetupAttachment(SceneComponent);

	GraphicNodes = CreateDefaultSubobject<USceneComponent>(TEXT("Graphic Nodes"));
	GraphicNodes->SetupAttachment(SceneComponent);
}

void AShot::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
