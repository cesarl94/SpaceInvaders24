// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "Actors/ActorInTexels.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GAS/GASEnums.h"
#include "GameplayTagContainer.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "Structs/BlastTrailData.h"
#include "Structs/GunData.h"
#include "Utils/Enums.h"

#include "LaserTank.generated.h"



class ALaserTank;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerSimpleEvent);

UCLASS()
class SPACEINVADERS24_API ALaserTank : public AActorInTexels, public IAbilitySystemInterface {
	GENERATED_BODY()

private:
#pragma region // GAS
	UPROPERTY()
	bool bIsInputBound{false};

	UFUNCTION()
	void InitializeGAS();

	UFUNCTION()
	void InitializeAbilities();

	UFUNCTION()
	void ApplyDefaultEffects();

	// To add mapping context to GAS
	UFUNCTION()
	void BindInput();

	UFUNCTION()
	void OnAttributeChanged(EPlayerAttribute AttributeEnum, float OldValue, float NewValue);
#pragma endregion

	UPROPERTY()
	float CurrentHorizontalMovement{0};

	UPROPERTY()
	FTimerHandle InputTimeHandle;

	UPROPERTY()
	class ABlastTrail *BlastTrailA;

	UPROPERTY()
	class ABlastTrail *BlastTrailB;

	UFUNCTION()
	void SpawnBlastTrails();

protected:
	// Components:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UAbilitySystemComponent *AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	class ULaserTankAttributeSet *AttributeSet;

	// Serialized data:
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Laser Tank Data")
	FGunData GunData;

	// texels per frame
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Laser Tank Data")
	float MovementSpeed{1};

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: GAS")
	TArray<TSubclassOf<class UCustomGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: GAS")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;

	// Since LaserTank has a death animation of 2 frames, BlastTrailDataA and B represent each frame. Ignore Duration, as there is a new Duration below.
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Laser Tank Blast Trail Data")
	FBlastTrailData BlastTrailDataA;

	// Since LaserTank has a death animation of 2 frames, BlastTrailDataA and B represent each frame. Ignore Duration, as there is a new Duration below.
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Laser Tank Blast Trail Data")
	FBlastTrailData BlastTrailDataB;

	// Time that elapses between each frame, in seconds.
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Laser Tank Blast Trail Data")
	float SwapBlastTrailFrequence{0.05f};

	// Total times that we'll see blast trail changes
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Laser Tank Blast Trail Data")
	int32 SwapBlastTrailCount = 11;

	// for binding the enhanced input system with GAS
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

public:
	ALaserTank();

	void ManualInitialize();

	// TODO: inprove this comment.
	// HardReset means: true for new games, false for new levels. If you die and want revive, use Revive() instead this
	void ManualReset(bool HardReset);

	void ManualTick(float DeltaTime);

	bool CanRevive() const;

	void Revive(bool RemoveLife);

	void PassLevel(FIntPoint NewTexelPosition);

	UFUNCTION(BlueprintCallable, Category = "Laser Tank", DisplayName = "LaserTank IsAlive", Meta = (CompactNodeTitle = "Is alive?"))
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Laser Tank", DisplayName = "LaserTank GetGunData", Meta = (CompactNodeTitle = "GUN Data"))
	FGunData GetGunData() const;

	UFUNCTION(BlueprintCallable, Category = "Laser Tank", DisplayName = "LaserTank CanShoot", Meta = (CompactNodeTitle = "Can Shoot?"))
	bool CanShot() const;

	UFUNCTION(BlueprintCallable, Category = "Laser Tank", DisplayName = "LaserTank Kill")
	void Kill(bool IsForcedKill = false);

	// Inherited from interface
	virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

	// Gets the Custom Ability System Component
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: GAS", DisplayName = "LaserTank GetCustomAbilitySystemComponent", Meta = (CompactNodeTitle = "ASC"))
	class UCustomAbilitySystemComponent *GetCustomAbilitySystemComponent() const;

	// Called from Player Controller to move this
	UFUNCTION(BlueprintCallable, Category = "Laser Tank", DisplayName = "LaserTank SetHorizontalMovement")
	void SetHorizontalMovement(float HorizontalMovement);

	// Temporal function until I activate the Enhanced input system
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: GAS", DisplayName = "LaserTank SetAbilityInput")
	void SetAbilityInput(EGASAbilityInput input, bool pressed);

	// Events:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FOnPlayerSimpleEvent OnStartDying;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FOnPlayerSimpleEvent OnFinallyDie;
};
