// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Shot.h"

#include "Actors/Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Core/GS_SpaceInvaders24.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/Enums.h"


void AShot::OnBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult) {
	if (GetType() == EShotType::SIMPLE_LINE) {

		if (AEnemy *HitEnemy = Cast<AEnemy>(OtherActor)) {
			HitEnemy->Kill();
			Dissapear();
		}
	}
}


void AShot::OnBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {}


void AShot::TouchLimit(EDirection Direction) { Dissapear(); }

void AShot::Dissapear() {
	OnHit.Broadcast(this);
	Destroy();
}

void AShot::BeginPlay() {
	Super::BeginPlay();

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));
	GameState->ReportNewShot(this);

	OnTouchLimit.AddUniqueDynamic(this, &AShot::TouchLimit);
}

AShot::AShot() {
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SceneComponent->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Collider->SetupAttachment(SceneComponent);

	GraphicNodes = CreateDefaultSubobject<USceneComponent>(TEXT("Graphic Nodes"));
	GraphicNodes->SetupAttachment(SceneComponent);

	Collider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AShot::OnBoxBeginOverlap);
	Collider->OnComponentEndOverlap.AddUniqueDynamic(this, &AShot::OnBoxEndOverlap);
}

void AShot::ManualTick(float DeltaTime) { ApplyVelocity(DeltaTime); }

EShotType AShot::GetType() const { return Type; }