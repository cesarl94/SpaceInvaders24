#pragma once

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)                                                                                                                                                   \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)                                                                                                                                         \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                                                                                                                                                       \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                                                                                                                                                       \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CoreMinimal.h"
#include "GAS/GASEnums.h"

#include "CustomAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeChanged, EPlayerAttribute, AttributeEnum, float, OldValue, float, NewValue);

UCLASS()
class SPACEINVADERS24_API UCustomAttributeSet : public UAttributeSet {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "SpaceInvaders24: GAS", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Crystals{0};
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Crystals)

	UPROPERTY(BlueprintReadOnly, Category = "SpaceInvaders24: GAS", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxCrystals{3};
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, MaxCrystals)

	UPROPERTY(BlueprintReadOnly, Category = "SpaceInvaders24: GAS", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Points{0};
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Points)

	UPROPERTY(BlueprintReadOnly, Category = "SpaceInvaders24: GAS", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Lives{3};
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Lives)

	UPROPERTY(BlueprintReadOnly, Category = "SpaceInvaders24: GAS", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Level{0};
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Level)

	FGameplayAttribute GetAttributeByEnum(EPlayerAttribute AttributeEnum);

	void ClampAttributeOnChange(const FGameplayAttribute &Attribute, float &NewValue) const;
	virtual void PreAttributeBaseChange(const FGameplayAttribute &Attribute, float &NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute &Attribute, float OldValue, float NewValue) override;

	// Events:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FAttributeChanged OnAttributeChange;
};
