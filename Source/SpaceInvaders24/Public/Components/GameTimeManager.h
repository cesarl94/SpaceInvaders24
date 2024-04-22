// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Structs/TimeStateData.h"
#include "Utils/Enums.h"

#include "GameTimeManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameTimeManagerEvent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEINVADERS24_API UGameTimeManager : public UActorComponent {
	GENERATED_BODY()

private:
	UPROPERTY()
	ETimeState TimeState{ETimeState::IDLE};

	UPROPERTY()
	float GlobalTimeAtLastStateChange{0};

	UPROPERTY()
	float NormalGameTotalSeconds{0};

	UPROPERTY()
	float CrystalTotalSeconds{0};

	UPROPERTY()
	float LastDeltaTime{0};

	UPROPERTY()
	float LastCrystalDeltaTime{0};

protected:
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Time Manager")
	TMap<ETimeState, FTimeStateData> TimeStateData;

public:
	UGameTimeManager();

	// Called from GS_SpaceInvaders24
	UFUNCTION()
	void ManualTick(float DeltaTime);

	// Returns the maximum time that we could pass in another time state (only special states count)
	UFUNCTION()
	float GetMaxTimeDilationDuration();

	UFUNCTION(BlueprintCallable)
	ETimeState GetTimeState();

	UFUNCTION(BlueprintCallable)
	void SetNewState(ETimeState NewTimeState);

	UFUNCTION(BlueprintCallable)
	float GetNormalGameTotalSeconds();

	UFUNCTION(BlueprintCallable)
	float GetCrystalTotalSeconds();

	UFUNCTION(BlueprintCallable)
	float GetLastDeltaTime();

	UFUNCTION(BlueprintCallable)
	float GetLastCrystalDeltaTime();


	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FGameTimeManagerEvent OnTimeStateFinished;
};
