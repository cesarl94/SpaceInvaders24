#include "GAS/CustomAttributeSet.h"

#include "GAS/GASEnums.h"

FGameplayAttribute UCustomAttributeSet::GetAttributeByEnum(EPlayerAttribute AttributeEnum) {
	switch (AttributeEnum) {
	case EPlayerAttribute::Crystals:
		return GetCrystalsAttribute();
	case EPlayerAttribute::Points:
		return GetPointsAttribute();
	case EPlayerAttribute::Lives:
		return GetLivesAttribute();
	case EPlayerAttribute::Level:
		return GetLevelAttribute();
	}
	return nullptr;
}

void UCustomAttributeSet::ClampAttributeOnChange(const FGameplayAttribute &Attribute, float &NewValue) const {
	if (Attribute == GetCrystalsAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0, GetMaxCrystals());
	}
}

void UCustomAttributeSet::PreAttributeBaseChange(const FGameplayAttribute &Attribute, float &NewValue) const {
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttributeOnChange(Attribute, NewValue);
}

void UCustomAttributeSet::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) {
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttributeOnChange(Attribute, NewValue);
}

void UCustomAttributeSet::PostAttributeChange(const FGameplayAttribute &Attribute, float OldValue, float NewValue) {
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (OldValue == NewValue) {
		return;
	}

	EPlayerAttribute AttributeEnum;
	if (Attribute == GetCrystalsAttribute()) {
		AttributeEnum = EPlayerAttribute::Crystals;
	} else if (Attribute == GetPointsAttribute()) {
		AttributeEnum = EPlayerAttribute::Points;
	} else if (Attribute == GetLivesAttribute()) {
		AttributeEnum = EPlayerAttribute::Lives;
	} else if (Attribute == GetLevelAttribute()) {
		AttributeEnum = EPlayerAttribute::Level;
	} else {
		return;
	}

	OnAttributeChange.Broadcast(AttributeEnum, OldValue, NewValue);
}