#include "GAS/CustomGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Logging/StructuredLog.h"
#include "Utils/Enums.h"


void UCustomGameplayAbility::OnAbilityGiven() { K2_OnAbilityGiven(); }

void UCustomGameplayAbility::OnAbilityRemoved() { K2_OnAbilityRemoved(); }

void UCustomGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec) {
	Super::OnAvatarSet(ActorInfo, Spec);

	if (AbilityInputID == EGASAbilityInput::None) {
		UE_LOGFMT(LogTemp, Log, "Invalid Ability Input ID. Ability Canceled");
		ActorInfo->AbilitySystemComponent->ClearAbility(Spec.Handle);
	} else if (bActivateAbilityOnGranted) {
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

const FGameplayTagContainer *UCustomGameplayAbility::GetCooldownTags() const {
	FGameplayTagContainer *MutableTags = const_cast<FGameplayTagContainer *>(&TempCooldownTags);
	MutableTags->Reset(); // MutableTags writes to the TempCooldownTags on the CDO so clear it in case the ability cooldown tags change (moved to a different slot)
	const FGameplayTagContainer *ParentTags = Super::GetCooldownTags();
	if (ParentTags) {
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}

void UCustomGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const {
	UGameplayEffect *CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE) {
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Duration")), CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}
