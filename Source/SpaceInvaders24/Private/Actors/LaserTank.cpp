

#include "Actors/LaserTank.h"

#include "AbilitySystemComponent.h"
#include "Actors/BlastTrail.h"
#include "Actors/Shot.h"
#include "Core/GS_SpaceInvaders24.h"
#include "Engine/EngineTypes.h"
#include "FCTween.h"
#include "FCTweenInstance.h"
#include "GAS/CustomAbilitySystemComponent.h"
#include "GAS/CustomGameplayAbility.h"
#include "GAS/GASEnums.h"
#include "GAS/LaserTankAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Vector.h"
#include "Structs/GunData.h"
#include "Utils/Enums.h"


void ALaserTank::InitializeGAS() {
	UCustomAbilitySystemComponent *ASC = GetCustomAbilitySystemComponent();
	if (ASC == nullptr) {
		return;
	}

	ASC->SetAttributeSetReference(AttributeSet);
	ASC->InitAbilityActorInfo(this, this);
	ASC->OnAttributeChange.AddUniqueDynamic(this, &ALaserTank::OnAttributeChanged);

	InitializeAbilities();

	BindInput();
}

void ALaserTank::InitializeAbilities() {
	for (TSubclassOf<UCustomGameplayAbility> &Ability : DefaultAbilities) {
		const EGASAbilityInput AbilityInputID = Ability.GetDefaultObject()->AbilityInputID;

		if (AbilityInputID == EGASAbilityInput::None) {
			UKismetSystemLibrary::PrintString(GetWorld(), "You're trying to set a Default Ability with \"None\" as Ability Input ID. This ability was canceled.", true, true, FColor::Red, 5);
		} else {
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(AbilityInputID), this));
		}
	}
}

void ALaserTank::ApplyDefaultEffects() {
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> &Effect : DefaultEffects) {
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (SpecHandle.IsValid()) {
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ALaserTank::BindInput() {
	if (bIsInputBound || !AbilitySystemComponent || !IsValid(InputComponent)) {
		return;
	}

	FTopLevelAssetPath EnumAssetPath = FTopLevelAssetPath(FName("/Script/SpaceInvaders24"), FName("EGASAbilityInput"));
	AbilitySystemComponent->BindAbilityActivationToInputComponent(
		InputComponent, FGameplayAbilityInputBinds(FString("Confirm"), FString("Cancel"), EnumAssetPath, static_cast<int32>(EGASAbilityInput::Confirm), static_cast<int32>(EGASAbilityInput::Cancel)));

	bIsInputBound = true;
}

void ALaserTank::OnAttributeChanged(EPlayerAttribute AttributeEnum, float OldValue, float NewValue) {
	if (AttributeEnum == EPlayerAttribute::Health && FMath::RoundToInt(NewValue) == 0) {
		Kill(false);
	}
}

void ALaserTank::SpawnBlastTrails() {
	if (BlastTrailDataA.BlastTrailActorClass == nullptr || BlastTrailDataB.BlastTrailActorClass == nullptr) {
		return;
	}

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	BlastTrailA = GetWorld()->SpawnActor<ABlastTrail>(BlastTrailDataA.BlastTrailActorClass, FVector(0, 0, 0), GameState->GetGameObjectOrientation(), ActorSpawnParams);
	BlastTrailB = GetWorld()->SpawnActor<ABlastTrail>(BlastTrailDataB.BlastTrailActorClass, FVector(0, 0, 0), GameState->GetGameObjectOrientation(), ActorSpawnParams);

	BlastTrailA->SetVisibility(false);
	BlastTrailB->SetVisibility(false);
}

void ALaserTank::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) {
	// Set up action bindings
	// if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
	// 	//Jumping
	// 	EnhancedInputComponent->BindAction (JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	// 	EnhancedInputComponent->BindAction (JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	// 	//Moving
	// 	EnhancedInputComponent->BindAction (MoveAction, ETriggerEvent:: Triggered, this, &ACKCharacter::Move);
	// 	//Looking
	// 	EnhancedInputComponent->BindAction (LookAction, ETriggerEvent::Triggered, this, &AGAS_GameCharacter::Look);
	// }

	BindInput();
}

ALaserTank::ALaserTank() : Super() {
	// GAS Components:
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>("Ability System Component");
	AttributeSet = CreateDefaultSubobject<ULaserTankAttributeSet>("Attribute Set");
}

void ALaserTank::ManualInitialize() {
	SetVisibility(false);

	InitializeGAS();

	SpawnBlastTrails();
}

void ALaserTank::ManualReset(bool HardReset) {
	if (HardReset) {
		ApplyDefaultEffects();
	}
	Revive(HardReset);
}

void ALaserTank::ManualTick(float DeltaTime) {
	if (IsAlive()) {
		ApplyVelocity(DeltaTime);
	}
}

bool ALaserTank::CanRevive() const {
	int32 PlayerLives = GetCustomAbilitySystemComponent()->GetAttributeValueByEnum(EPlayerAttribute::Lives);

	return PlayerLives - 1 >= 0;
}

void ALaserTank::Revive(bool RemoveLife) {
	if (RemoveLife && !CanRevive()) {
		return;
	}

	GetCustomAbilitySystemComponent()->AddTagByString("Player.IsAlive");
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetVisibility(true);

	// TODO: Improve this.
	GetCustomAbilitySystemComponent()->SetAttributeValueByEnum(EPlayerAttribute::Health, 1);

	if (RemoveLife) {
		GetCustomAbilitySystemComponent()->AddToAttributeValueByEnum(EPlayerAttribute::Lives, -1);
	}
}

void ALaserTank::PassLevel(FIntPoint NewTexelPosition) { SetTexelPosition(NewTexelPosition, false); }

bool ALaserTank::IsAlive() const { return GetCustomAbilitySystemComponent()->HasTagByString("Player.IsAlive"); }

FGunData ALaserTank::GetGunData() const { return GunData; }

bool ALaserTank::CanShot() const {
	if (!IsAlive()) {
		return false;
	}

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));


	// Yes, this is weird, but works. Above, the valid GameStates, wherever else will return false
	switch (GameState->GetGameState()) {
	case EGameState::PLAYING_FORWARD:
	case EGameState::PLAYING_SLOW_TIME_DOWN:
	case EGameState::PLAYING_PAUSED_TIME:
	case EGameState::PLAYING_REVERSE:
		break;
	default:
		return false;
	}

	// If exists any other shot that is SIMPLE_LINE, we'll return false
	for (AShot *Shot : GameState->GetShots()) {
		if (Shot->GetType() == EShotType::SIMPLE_LINE) {
			return false;
		}
	}

	return true;
}

void ALaserTank::Kill(bool IsForcedKill) {
	if (!IsAlive()) {
		return;
	}
	if (IsForcedKill) {

		return;
	}
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetVisibility(false);
	BlastTrailA->SetVisibility(true);

	GetCustomAbilitySystemComponent()->RemoveTagByString("Player.IsAlive");
	GetCustomAbilitySystemComponent()->SetAttributeValueByEnum(EPlayerAttribute::Crystals, 0);

	BlastTrailA->SetTexelPosition(GetFloatTexelPosition());
	BlastTrailB->SetTexelPosition(GetFloatTexelPosition());

	FCTween::Play(
		0, 1, [&](float t) {}, SwapBlastTrailFrequence, EFCEase::Linear)
		->SetLoops(SwapBlastTrailCount)
		->SetOnLoop([&]() {
			BlastTrailA->ToggleVisibility();
			BlastTrailB->ToggleVisibility();
		})
		->SetOnComplete([&]() {
			BlastTrailA->SetVisibility(false);
			BlastTrailB->SetVisibility(false);
			GetWorld()->GetTimerManager().SetTimer(
				InputTimeHandle, [&]() { OnFinallyDie.Broadcast(); }, 0.5f, false);
		});

	OnStartDying.Broadcast();
}

UAbilitySystemComponent *ALaserTank::GetAbilitySystemComponent() const { return AbilitySystemComponent; }

UCustomAbilitySystemComponent *ALaserTank::GetCustomAbilitySystemComponent() const { return Cast<UCustomAbilitySystemComponent>(AbilitySystemComponent); }

void ALaserTank::SetHorizontalMovement(float HorizontalMovement) {
	if (HorizontalMovement == CurrentHorizontalMovement) {
		return;
	}

	CurrentHorizontalMovement = HorizontalMovement;

	// We multiply by 60 because the Speed is in texels per frame
	SetTexelVelocity(FVector2D(HorizontalMovement * MovementSpeed * 60.f, 0));
}

void ALaserTank::SetAbilityInput(EGASAbilityInput input, bool pressed) {
	if (AbilitySystemComponent == nullptr) {
		return;
	}

	int32 inputId = static_cast<int32>(input);

	if (pressed) {
		AbilitySystemComponent->AbilityLocalInputPressed(inputId);
	} else {
		AbilitySystemComponent->AbilityLocalInputReleased(inputId);
	}
}