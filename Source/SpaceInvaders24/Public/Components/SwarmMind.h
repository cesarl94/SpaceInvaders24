// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "SwarmMind.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEINVADERS24_API USwarmMind : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USwarmMind();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called from GS_SpaceInvaders24
	UFUNCTION()
	void ManualTick(float DeltaTime);
};
