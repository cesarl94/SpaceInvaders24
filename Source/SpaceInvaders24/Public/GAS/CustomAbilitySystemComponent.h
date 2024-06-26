// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "GAS/GASEnums.h"
#include "GameplayTagContainer.h"

#include "CustomAbilitySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTagUpdated, const FGameplayTag &, Tag, bool, TagExists);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeChanged2, EPlayerAttribute, AttributeEnum, float, OldValue, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActivateAbility, UGameplayAbility *, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCommitedAbility, UGameplayAbility *, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEndAbility, UGameplayAbility *, Ability, bool, WasCancelled);


UCLASS()
class SPACEINVADERS24_API UCustomAbilitySystemComponent : public UAbilitySystemComponent {
	GENERATED_BODY()

private:
	UPROPERTY()
	class UCustomAttributeSet *CustomAttributeSet;

	UFUNCTION()
	void OnAttributeChanged(EPlayerAttribute AttributeEnum, float OldValue, float NewValue);

protected:
	virtual void OnTagUpdated(const FGameplayTag &Tag, bool TagExists) override;

	/** Will be called from GiveAbility or from OnRep. Initializes events (triggers and inputs) with the given ability */
	virtual void OnGiveAbility(FGameplayAbilitySpec &AbilitySpec) override;

	/** Will be called from RemoveAbility or from OnRep. Unbinds inputs with the given ability */
	virtual void OnRemoveAbility(FGameplayAbilitySpec &AbilitySpec) override;

public:
	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility *Ability) override;
	virtual void NotifyAbilityCommit(UGameplayAbility *Ability) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility *Ability, bool bWasCancelled) override;

	// This is ugly, and seems innecesary, but is an exception for get the GameState in a place where I haven't world context but I have a reference to an object of this class
	// I talk about the function "CanActivateAbility" of the "CustomGameplayAbility"
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: GAS", DisplayName = "CustomAbilitySystemComponent GetGameState", Meta = (CompactNodeTitle = "GameState"))
	class AGameStateBase *GetGameState() const;

	// Adds a Gameplay Ability Tag to Custom Ability System Component
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: GAS")
	void AddTag(FGameplayTag Tag);

	// Adds a Gameplay Ability Tag to Custom Ability System Component by a FString
	UFUNCTION()
	void AddTagByString(FString TagString);

	// Removes a Gameplay Ability Tag to Custom Ability System Component
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: GAS")
	void RemoveTag(FGameplayTag Tag);

	// Removes a Gameplay Ability Tag to Custom Ability System Component by a FString
	UFUNCTION()
	void RemoveTagByString(FString TagString);

	// Returns true if there is a Gameplay Ability Tag matching with the passed tag in the Tag Container of the Custom Ability System Component
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: GAS")
	bool HasTag(FGameplayTag Tag) const;

	/**
	 * Checks if there's a Gameplay Ability Tag matching with any Tag in the Tag Container by a String.
	 * @param TagString The name of the Gameplay Ability Tag. E.g. "Turbo.Drift"
	 * @param Contains If it's true, we'll check only if the passed TagString is contained in any Tag in the Tag Container. If it's false, we'll check if the
	 * TagString matches exactly. E.g. we pass "Turbo", and Contains in "true", and we have a tag called "Turbo.Drift", so the function will returns true.
	 * @return if there is some Gameplay Ability Tag matching with the passed parameters.
	 */
	UFUNCTION()
	bool HasTagByString(FString TagString, bool Contains = false) const;

	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: GAS")
	float GetAttributeValueByEnum(EPlayerAttribute AttributeEnum) const;

	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: GAS")
	void SetAttributeValueByEnum(EPlayerAttribute AttributeEnum, float Value);

	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: GAS")
	void AddToAttributeValueByEnum(EPlayerAttribute AttributeEnum, float ValueToAdd);

	void SetAttributeSetReference(class UCustomAttributeSet *AttributeSet);

	// Events:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FOnTagUpdated OnTagUpdateEvt;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FAttributeChanged2 OnAttributeChange;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FActivateAbility OnAbilityActivated;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FCommitedAbility OnAbilityCommited;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FEndAbility OnAbilityEnded;
};
