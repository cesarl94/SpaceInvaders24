#include "GAS/CustomAttributeSet.h"

#include "GAS/GASEnums.h"

FGameplayAttribute UCustomAttributeSet::GetAttributeByEnum(EPlayerAttribute AttributeEnum) {
	if (AttributeEnum == EPlayerAttribute::Health) {
		return GetHealthAttribute();
	}
	return nullptr;
}

void UCustomAttributeSet::PostAttributeChange(const FGameplayAttribute &Attribute, float OldValue, float NewValue) {
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (OldValue != NewValue && Attribute == GetHealthAttribute()) {
		OnAttributeChange.Broadcast(EPlayerAttribute::Health, OldValue, NewValue);
	}
}