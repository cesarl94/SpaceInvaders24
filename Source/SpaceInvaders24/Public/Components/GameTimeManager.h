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
	TMap<EGameState, FTimeStateData> TimeStateData;

public:
	UGameTimeManager();

	UFUNCTION()
	void ManualReset();

	// Called from GS_SpaceInvaders24
	UFUNCTION()
	void ManualTick(float DeltaTime);

	// Called from GS
	UFUNCTION()
	void OnNewGameState(EGameState NewGameState);

	// Returns the maximum time that we could pass in another time state (only special states count)
	UFUNCTION()
	float GetDurationOfLongestTimeState();

	// Obtains the elapsed time since this game started.
	UFUNCTION(BlueprintCallable)
	float GetNormalGameTotalSeconds();

	// Gets the elapsed time since this game started, affected by crystals.
	UFUNCTION(BlueprintCallable)
	float GetCrystalTotalSeconds();

	// Gets the last delta time.
	UFUNCTION(BlueprintCallable)
	float GetLastDeltaTime();

	// Gets the last delta time, affected by crystals.
	UFUNCTION(BlueprintCallable)
	float GetLastCrystalDeltaTime();

	// This event is triggered when the current time state ends, as long as it is a state that has a duration.
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FGameTimeManagerEvent OnTimeStateFinished;
};
