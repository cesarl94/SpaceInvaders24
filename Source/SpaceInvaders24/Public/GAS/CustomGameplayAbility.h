// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "GAS/CustomAbilitySystemComponent.h"
#include "Utils/Enums.h"

#include "CustomGameplayAbility.generated.h"

UCLASS()
class SPACEINVADERS24_API UCustomGameplayAbility : public UGameplayAbility {
	GENERATED_BODY()

private:
	friend class UCustomAbilitySystemComponent;

	// Called from CustomAbilitySystemComponent
	void OnAbilityGiven();

	// Called from CustomAbilitySystemComponent
	void OnAbilityRemoved();

	// Temp container that we will return the pointer to in GetCooldownTags().
	// This will be a union of our CooldownTags and the Cooldown GE's cooldown tags.
	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: GAS")
	EGASAbilityInput AbilityInputID{EGASAbilityInput::None};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: GAS")
	FScalableFloat CooldownDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: GAS")
	FGameplayTagContainer CooldownTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: GAS")
	bool bActivateAbilityOnGranted{false};

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "CustomGameplayAbility OnAbilityGiven")
	void K2_OnAbilityGiven();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "CustomGameplayAbility OnAbilityRemoved")
	void K2_OnAbilityRemoved();

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec) override;
	virtual const FGameplayTagContainer *GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
};