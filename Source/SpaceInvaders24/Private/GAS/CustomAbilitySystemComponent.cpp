#include "GAS/CustomAbilitySystemComponent.h"

#include "GAS/CustomAttributeSet.h"
#include "GAS/CustomGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"
#include "Utils/Enums.h"

void UCustomAbilitySystemComponent::AddTag(FGameplayTag Tag) { AddLooseGameplayTag(Tag); }

void UCustomAbilitySystemComponent::AddTagByString(FString TagString) { AddTag(FGameplayTag::RequestGameplayTag(FName(TagString))); }

void UCustomAbilitySystemComponent::RemoveTag(FGameplayTag Tag) { RemoveLooseGameplayTag(Tag); }

void UCustomAbilitySystemComponent::RemoveTagByString(FString TagString) { RemoveTag(FGameplayTag::RequestGameplayTag(FName(TagString))); }

bool UCustomAbilitySystemComponent::HasTag(FGameplayTag Tag) const { return HasMatchingGameplayTag(Tag); }

bool UCustomAbilitySystemComponent::HasTagByString(FString TagString, bool Contains) const {
	if (Contains) {
		return GameplayTagCountContainer.GetExplicitGameplayTags().HasTag(FGameplayTag::RequestGameplayTag(FName(TagString)));
	} else {
		return GameplayTagCountContainer.GetExplicitGameplayTags().HasTagExact(FGameplayTag::RequestGameplayTag(FName(TagString)));
	}
}

void UCustomAbilitySystemComponent::SetAttributeSetReference(UCustomAttributeSet *AttributeSet) { CustomAttributeSet = AttributeSet; }

void UCustomAbilitySystemComponent::OnTagUpdated(const FGameplayTag &Tag, bool TagExists) {
	// UE_LOGFMT(LogTemp, Log, "TagName: {0}, {1}", Tag.GetTagName(), TagExists);

	OnTagUpdateEvt.Broadcast(Tag, TagExists);
}

void UCustomAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec &AbilitySpec) {
	Super::OnGiveAbility(AbilitySpec);

	UCustomGameplayAbility *CustomAbility = Cast<UCustomGameplayAbility>(AbilitySpec.Ability);
	if (CustomAbility != nullptr) {
		CustomAbility->OnAbilityGiven();
	}
}

void UCustomAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec &AbilitySpec) {
	Super::OnRemoveAbility(AbilitySpec);

	UCustomGameplayAbility *CustomAbility = Cast<UCustomGameplayAbility>(AbilitySpec.Ability);
	if (CustomAbility != nullptr) {
		CustomAbility->OnAbilityRemoved();
	}
}

float UCustomAbilitySystemComponent::GetAttributeValueByEnum(EPlayerAttribute AttributeEnum) const {
	FGameplayAttribute Attribute = CustomAttributeSet->GetAttributeByEnum(AttributeEnum);
	return GetNumericAttribute(Attribute);
}

void UCustomAbilitySystemComponent::SetAttributeValueByEnum(EPlayerAttribute AttributeEnum, float Value) {
	FGameplayAttribute Attribute = CustomAttributeSet->GetAttributeByEnum(AttributeEnum);
	SetNumericAttributeBase(Attribute, Value);
}