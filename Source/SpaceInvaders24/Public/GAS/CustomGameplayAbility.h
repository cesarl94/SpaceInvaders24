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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: GAS")
	EGASAbilityInput AbilityInputID{EGASAbilityInput::None};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: GAS")
	bool bActivateAbilityOnGranted{false};

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "CustomGameplayAbility OnAbilityGiven")
	void K2_OnAbilityGiven();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "CustomGameplayAbility OnAbilityRemoved")
	void K2_OnAbilityRemoved();

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec) override;
};