#include "GAS/CustomAbilitySystemComponent.h"

#include "GAS/CustomAttributeSet.h"
#include "GAS/CustomGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"
#include "Utils/Enums.h"


AGameStateBase *UCustomAbilitySystemComponent::GetGameState() const { return UGameplayStatics::GetGameState(this); }

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

void UCustomAbilitySystemComponent::OnAttributeChanged(EPlayerAttribute AttributeEnum, float OldValue, float NewValue) {
	OnAttributeChange.Broadcast(AttributeEnum, OldValue, NewValue);
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("New Crystal count: %d"), static_cast<int32>(NewValue), OldValue, NewValue), true, true, FColor::Yellow, 5);
}

void UCustomAbilitySystemComponent::OnTagUpdated(const FGameplayTag &Tag, bool TagExists) { OnTagUpdateEvt.Broadcast(Tag, TagExists); }

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

void UCustomAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility *Ability) {
	Super::NotifyAbilityActivated(Handle, Ability);

	if (UCustomGameplayAbility *GA = Cast<UCustomGameplayAbility>(Ability)) {
		OnAbilityActivated.Broadcast(GA);
	}
}

void UCustomAbilitySystemComponent::NotifyAbilityCommit(UGameplayAbility *Ability) {
	Super::NotifyAbilityCommit(Ability);

	if (UCustomGameplayAbility *GA = Cast<UCustomGameplayAbility>(Ability)) {
		OnAbilityCommited.Broadcast(GA);
	}
}

void UCustomAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility *Ability, bool bWasCancelled) {
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);

	if (UCustomGameplayAbility *GA = Cast<UCustomGameplayAbility>(Ability)) {
		OnAbilityEnded.Broadcast(GA, bWasCancelled);
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

void UCustomAbilitySystemComponent::SetAttributeSetReference(UCustomAttributeSet *AttributeSet) {
	CustomAttributeSet = AttributeSet;
	CustomAttributeSet->OnAttributeChange.AddUniqueDynamic(this, &UCustomAbilitySystemComponent::OnAttributeChanged);
}
