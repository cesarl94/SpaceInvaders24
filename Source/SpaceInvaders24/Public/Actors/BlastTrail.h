// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/ActorInTexels.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"

#include "BlastTrail.generated.h"



UCLASS()
class SPACEINVADERS24_API ABlastTrail : public AActorInTexels {
	GENERATED_BODY()

private:
	UPROPERTY()
	FTimerHandle InputTimeHandle;

protected:
	// Components:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *GraphicNodes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent *Collider;

public:
	ABlastTrail();

	// Called from the class that spawned this actor after creation
	UFUNCTION()
	void ManualInitialize(float Duration);
};