// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "Actors/ActorInTexels.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GAS/GASEnums.h"
#include "Math/IntPoint.h"
#include "Structs/BlastTrailData.h"
#include "Structs/CrystalDropData.h"
#include "Structs/GunData.h"
#include "Utils/Enums.h"

#include "Enemy.generated.h"


class AEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyDie, AEnemy *, DiedEnemy, int32, Points);

UCLASS()
class SPACEINVADERS24_API AEnemy : public AActorInTexels, public IAbilitySystemInterface {
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
	class ABunker *OverlappedBunker;

	UPROPERTY()
	EEnemyState State{EEnemyState::IDLE};

	UPROPERTY()
	FIntPoint EnemyCoordinateInGrid;

	UPROPERTY()
	bool Alive{false};

	UPROPERTY()
	bool LastAnimationWasForward{false};

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SpawnBlastTrail();

	UFUNCTION()
	void SpawnCrystal();


protected:
	// Components:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UAbilitySystemComponent *AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	class UCustomAttributeSet *AttributeSet;

	// Serialized data:
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: GAS")
	TArray<TSubclassOf<class UCustomGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: GAS")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Stats")
	EEnemyType Type;

	// The amount of points that this enemy could give you will be taked randomly inside of this array.
	// E.g: A simple enemy should have only 1 element. UFO could have 3.
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Stats")
	TArray<int32> PointsThatCouldGive;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Gun Data")
	FGunData GunData;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Crystal Drop Data")
	FCrystalDropData CrystalDropData;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Blast Trace Data")
	FBlastTrailData BlastTrailData;

	// This should be implemented in Blueprints
	UFUNCTION(BlueprintNativeEvent, Category = "SpaceInvaders24: Native Events")
	void Animate(bool Forward, float Rate) const;


public:
	AEnemy();

	// Called from GS_SpaceInvaders24 after create
	void ManualInitialize(FIntPoint CoordinateInGrid);

	void ManualTick();

	UPROPERTY()
	float AnimationPlayRate{1};

	UFUNCTION()
	void TriggerMoveAnimation(float PlayRate);

	UFUNCTION()
	void ManualReset(FIntPoint NewTexelPosition);

	UFUNCTION()
	FIntPoint GetCoordinateInEnemyGrid() const;

	UFUNCTION(BlueprintCallable, DisplayName = "Enemy IsAlive")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, DisplayName = "Enemy GetGunData", Meta = (CompactNodeTitle = "GUN Data"))
	FGunData GetGunData() const;

	UFUNCTION()
	void Kill(bool IsForcedKill = false);

	// GAS:

	// Inherited from interface
	virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

	// Gets the Custom Ability System Component
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: GAS", DisplayName = "Enemy GetCustomAbilitySystemComponent", Meta = (CompactNodeTitle = "ASC"))
	class UCustomAbilitySystemComponent *GetCustomAbilitySystemComponent() const;

	// Temporal function until I activate the Enhanced input system
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: GAS", DisplayName = "Enemy SetAbilityInput")
	void SetAbilityInput(EGASAbilityInput input, bool pressed);

	// Events:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FOnEnemyDie OnDie;
};
