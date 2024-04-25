

#include "Actors/LaserTank.h"

#include "AbilitySystemComponent.h"
#include "Actors/Shot.h"
#include "Core/GS_SpaceInvaders24.h"
#include "GAS/CustomAbilitySystemComponent.h"
#include "GAS/CustomAttributeSet.h"
#include "GAS/CustomGameplayAbility.h"
#include "GAS/GASEnums.h"
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

	for (TSubclassOf<UCustomGameplayAbility> &Ability : DefaultAbilities) {
		const EGASAbilityInput AbilityInputID = Ability.GetDefaultObject()->AbilityInputID;

		if (AbilityInputID == EGASAbilityInput::None) {
			UKismetSystemLibrary::PrintString(GetWorld(), "You're trying to set a Default Ability with \"None\" as Ability Input ID. This ability was canceled.", true, true, FColor::Red, 5);
		} else {
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(AbilityInputID), this));
		}
	}

	BindInput();
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

void ALaserTank::BeginPlay() {
	Super::BeginPlay();

	GraphicNodes->SetVisibility(false, true);

	InitializeGAS();
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
	// GAS
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>("Ability System Component");
	AttributeSet = CreateDefaultSubobject<UCustomAttributeSet>("Attribute Set");
}

void ALaserTank::StartGame() {
	GetCustomAbilitySystemComponent()->AddTagByString("Player.IsAlive");
	GraphicNodes->SetVisibility(true, true);
}

void ALaserTank::ManualTick(float DeltaTime) {
	if (IsAlive()) {
		ApplyVelocity(DeltaTime);
	}
}

bool ALaserTank::IsAlive() const { return GetCustomAbilitySystemComponent()->HasTagByString("Player.IsAlive"); }

FGunData ALaserTank::GetGunData() const { return GunData; }

bool ALaserTank::CanShot() const {
	if (!IsAlive()) {
		return false;
	}

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));

	for (AShot *Shot : GameState->GetShots()) {
		if (Shot->GetType() == EShotType::SIMPLE_LINE) {
			return false;
		}
	}

	return true;
}

void ALaserTank::Kill(bool IsForcedKill) { OnDie.Broadcast(); }

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