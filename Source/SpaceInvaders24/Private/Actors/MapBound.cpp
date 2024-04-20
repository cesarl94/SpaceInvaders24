// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MapBound.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AMapBound::AMapBound() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SceneComponent->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	Mesh->SetupAttachment(SceneComponent);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Collider->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AMapBound::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AMapBound::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

EMapBoundType AMapBound::GetBoundType() const { return BoundType; }