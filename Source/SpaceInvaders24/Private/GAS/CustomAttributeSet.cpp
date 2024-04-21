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