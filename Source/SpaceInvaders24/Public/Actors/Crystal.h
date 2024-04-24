// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/ActorInTexels.h"
#include "CoreMinimal.h"
#include "Structs/BlastTrailData.h"

#include "Crystal.generated.h"

class ACrystal;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCrystalHit, ACrystal *, HitCrystal, bool, AddCrystalCount);

UCLASS()
class SPACEINVADERS24_API ACrystal : public AActorInTexels {
	GENERATED_BODY()

private:
	UFUNCTION()
	void SpawnBlastTrail();

protected:
	// Serialized data:
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Blast Trace Data")
	FBlastTrailData BlastTrailData;

public:
	// Called from Enemy after spawn it
	UFUNCTION()
	void ManualInitialize();

	/**
	 * Called from Shot after hit it, or from GS when resets the game
	 * @param IsForcedKill if it's true will not give crystals to player
	 */
	UFUNCTION()
	void Kill(bool IsForcedKill = false);

	// Events:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FOnCrystalHit OnHit;
};
