// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Utils/Enums.h"

#include "GameTimeManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEINVADERS24_API UGameTimeManager : public UActorComponent {
	GENERATED_BODY()

private:
	UPROPERTY()
	ETimeState TimeState{ETimeState::IDLE};


	UPROPERTY()
	float NormalGameTimeSeconds{0};

	UPROPERTY()
	float TimeAtLastStateChange{0};

	UPROPERTY()
	float CrystalTimeSeconds{0};

	UPROPERTY()
	float LastDeltaTime{0};

	UPROPERTY()
	float LastCrystalDeltaTime{0};

public:
	UGameTimeManager();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION()
	void SetNewState(ETimeState NewTimeState);
};
