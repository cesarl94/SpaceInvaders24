#include "GAS/LaserTankAttributeSet.h"

#include "GAS/GASEnums.h"


FGameplayAttribute ULaserTankAttributeSet::GetAttributeByEnum(EPlayerAttribute AttributeEnum) {
	switch (AttributeEnum) {
	case EPlayerAttribute::Crystals:
		return GetCrystalsAttribute();
	case EPlayerAttribute::MaxCrystals:
		return GetMaxCrystalsAttribute();
	case EPlayerAttribute::Points:
		return GetPointsAttribute();
	case EPlayerAttribute::Lives:
		return GetLivesAttribute();
	case EPlayerAttribute::Level:
		return GetLevelAttribute();
	}
	return Super::GetAttributeByEnum(AttributeEnum);
}

void ULaserTankAttributeSet::ClampAttributeOnChange(const FGameplayAttribute &Attribute, float &NewValue) const {
	if (Attribute == GetCrystalsAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0, GetMaxCrystals());
	}
}

void ULaserTankAttributeSet::PreAttributeBaseChange(const FGameplayAttribute &Attribute, float &NewValue) const {
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttributeOnChange(Attribute, NewValue);
}

void ULaserTankAttributeSet::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) {
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttributeOnChange(Attribute, NewValue);
}

void ULaserTankAttributeSet::PostAttributeChange(const FGameplayAttribute &Attribute, float OldValue, float NewValue) {
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (OldValue == NewValue) {
		return;
	}

	EPlayerAttribute AttributeEnum;
	if (Attribute == GetCrystalsAttribute()) {
		AttributeEnum = EPlayerAttribute::Crystals;
	} else if (Attribute == GetMaxCrystalsAttribute()) {
		AttributeEnum = EPlayerAttribute::MaxCrystals;
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