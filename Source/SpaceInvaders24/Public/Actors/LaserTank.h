// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "Actors/ActorInTexels.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "Structs/GunData.h"
#include "Utils/Enums.h"

#include "LaserTank.generated.h"


UCLASS()
class SPACEINVADERS24_API ALaserTank : public AActorInTexels, public IAbilitySystemInterface {
	GENERATED_BODY()

private:
	UPROPERTY()
	float CurrentHorizontalMovement{0};


#pragma region // GAS
	UPROPERTY()
	bool bIsInputBound{false};

	UFUNCTION()
	void InitializeGAS();

	// To add mapping context to GAS
	UFUNCTION()
	void BindInput();
#pragma endregion

protected:
	virtual void BeginPlay() override;

	// Components:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *GraphicNodes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent *Collider;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UAbilitySystemComponent *AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	class UCustomAttributeSet *AttributeSet;

	// Serialized data:
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Laser Tank Data")
	FGunData GunData;

	// texels per frame
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Laser Tank Data")
	float MovementSpeed{1};

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: GAS")
	TArray<TSubclassOf<class UCustomGameplayAbility>> DefaultAbilities;

	// for binding the enhanced input system with GAS
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

public:
	ALaserTank();

	void StartGame();

	void ManualTick(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Laser Tank", DisplayName = "LaserTank IsAlive", Meta = (CompactNodeTitle = "Is alive?"))
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Laser Tank", DisplayName = "LaserTank GetGunData", Meta = (CompactNodeTitle = "GUN Data"))
	FGunData GetGunData() const;

	UFUNCTION(BlueprintCallable, Category = "Laser Tank", DisplayName = "LaserTank CanShoot", Meta = (CompactNodeTitle = "Can Shoot?"))
	bool CanShot() const;

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
};
