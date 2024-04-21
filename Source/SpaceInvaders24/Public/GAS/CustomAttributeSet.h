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


UCLASS()
class SPACEINVADERS24_API UCustomAttributeSet : public UAttributeSet {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "SpaceInvaders24: GAS", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Crystals{0};
	ATTRIBUTE_ACCESSORS(UCustomAttributeSet, Crystals)

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
};
