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
#include "GAS/CustomAttributeSet.h"
#include "GAS/GASEnums.h"

#include "LaserTankAttributeSet.generated.h"


UCLASS()
class SPACEINVADERS24_API ULaserTankAttributeSet : public UCustomAttributeSet {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "SpaceInvaders24: GAS", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Crystals{0};
	ATTRIBUTE_ACCESSORS(ULaserTankAttributeSet, Crystals)

	UPROPERTY(BlueprintReadOnly, Category = "SpaceInvaders24: GAS", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxCrystals{3};
	ATTRIBUTE_ACCESSORS(ULaserTankAttributeSet, MaxCrystals)

	UPROPERTY(BlueprintReadOnly, Category = "SpaceInvaders24: GAS", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Points{0};
	ATTRIBUTE_ACCESSORS(ULaserTankAttributeSet, Points)

	UPROPERTY(BlueprintReadOnly, Category = "SpaceInvaders24: GAS", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Lives{3};
	ATTRIBUTE_ACCESSORS(ULaserTankAttributeSet, Lives)

	UPROPERTY(BlueprintReadOnly, Category = "SpaceInvaders24: GAS", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Level{0};
	ATTRIBUTE_ACCESSORS(ULaserTankAttributeSet, Level)

	virtual FGameplayAttribute GetAttributeByEnum(EPlayerAttribute AttributeEnum) override;

	void ClampAttributeOnChange(const FGameplayAttribute &Attribute, float &NewValue) const;
	virtual void PreAttributeBaseChange(const FGameplayAttribute &Attribute, float &NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute &Attribute, float OldValue, float NewValue) override;
};
