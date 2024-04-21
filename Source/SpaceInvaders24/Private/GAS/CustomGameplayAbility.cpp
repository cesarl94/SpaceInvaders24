#include "GAS/CustomGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Logging/StructuredLog.h"
#include "Utils/Enums.h"


void UCustomGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec) {
	Super::OnAvatarSet(ActorInfo, Spec);

	if (AbilityInputID == EGASAbilityInput::None) {
		UE_LOGFMT(LogTemp, Log, "Invalid Ability Input ID. Ability Canceled");
		ActorInfo->AbilitySystemComponent->ClearAbility(Spec.Handle);
	} else if (bActivateAbilityOnGranted) {
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UCustomGameplayAbility::OnAbilityGiven() { K2_OnAbilityGiven(); }

void UCustomGameplayAbility::OnAbilityRemoved() { K2_OnAbilityRemoved(); }