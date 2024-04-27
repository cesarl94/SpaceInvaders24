// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/ActorInTexels.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"

#include "BlastTrail.generated.h"

// TODO: Comentar esta clase

UCLASS()
class SPACEINVADERS24_API ABlastTrail : public AActorInTexels {
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void ErodeBunker(class ABunker *Bunker);

	UPROPERTY()
	FTimerHandle InputTimeHandle;

public:
	ABlastTrail();

	// Called from the class that spawned this actor after creation
	UFUNCTION()
	void ManualInitialize(float Duration);
};
