

#include "Actors/LaserTank.h"

#include "AbilitySystemComponent.h"
#include "Core/GS_SpaceInvaders24.h"
#include "GAS/CustomAbilitySystemComponent.h"
#include "GAS/CustomAttributeSet.h"
#include "GAS/GASEnums.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "Structs/GunData.h"


void ALaserTank::InitializeGAS() {
	UCustomAbilitySystemComponent *ASC = GetCustomAbilitySystemComponent();
	if (ASC == nullptr) {
		return;
	}

	ASC->SetAttributeSetReference(AttributeSet);
	ASC->InitAbilityActorInfo(this, this);

	BindInput();

	InitializeEffects();
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

void ALaserTank::InitializeEffects() {
	if (AbilitySystemComponent == nullptr) {
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> &Effect : DefaultEffects) {
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (SpecHandle.IsValid()) {
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
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

ALaserTank::ALaserTank() {
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SceneComponent->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Collider->SetupAttachment(SceneComponent);

	GraphicNodes = CreateDefaultSubobject<USceneComponent>(TEXT("Graphic Nodes"));
	GraphicNodes->SetupAttachment(SceneComponent);

	// GAS
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>("Ability System Component");
	AttributeSet = CreateDefaultSubobject<UCustomAttributeSet>("Attribute Set");
}

void ALaserTank::StartGame() {
	GetCustomAbilitySystemComponent()->AddTagByString("Player.IsAlive");
	GraphicNodes->SetVisibility(true, true);
}

bool ALaserTank::IsAlive() const { return GetCustomAbilitySystemComponent()->HasTagByString("Player.IsAlive"); }

FGunData ALaserTank::GetGunData() const { return GunData; }

UAbilitySystemComponent *ALaserTank::GetAbilitySystemComponent() const { return AbilitySystemComponent; }

UCustomAbilitySystemComponent *ALaserTank::GetCustomAbilitySystemComponent() const { return Cast<UCustomAbilitySystemComponent>(AbilitySystemComponent); }

void ALaserTank::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (IsAlive()) {
		ApplyVelocity(DeltaTime);
	}
}

void ALaserTank::SetHorizontalMovement(float HorizontalMovement) {
	if (HorizontalMovement == CurrentHorizontalMovement) {
		return;
	}

	CurrentHorizontalMovement = HorizontalMovement;

	// We multiply by 60 because the Speed is in texels per frame
	SetTexelVelocity(FVector2D(HorizontalMovement * Speed * 60.f, 0));
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