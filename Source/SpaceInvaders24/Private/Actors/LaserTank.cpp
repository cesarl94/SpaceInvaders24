

#include "Actors/LaserTank.h"

#include "AbilitySystemComponent.h"
#include "Core/GS_SpaceInvaders24.h"
#include "GAS/CustomAbilitySystemComponent.h"
#include "GAS/CustomAttributeSet.h"
#include "GAS/GASEnums.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"

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

void ALaserTank::PossessedBy(AController *NewController) {
	Super::PossessedBy(NewController);

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));
	CurrentTexelPosition = GameState->WorldToTexelPos(RootComponent->GetComponentLocation());

	// FVector Texel0X0Y_WorldPos = GameState->TexelToWorldPos(FIntPoint(0, 0));
	// FVector Texel1X0Y_WorldPos = GameState->TexelToWorldPos(FIntPoint(1, 0));
	// FVector WorldDifferenceBetweenTexels = Texel1X0Y_WorldPos - Texel0X0Y_WorldPos;
	// float TexelWorldDifference = WorldDifferenceBetweenTexels.Size();
	// FVector HorizontalMovementWorldDirection = WorldDifferenceBetweenTexels / TexelWorldDifference;
}

UAbilitySystemComponent *ALaserTank::GetAbilitySystemComponent() const { return AbilitySystemComponent; }

UCustomAbilitySystemComponent *ALaserTank::GetCustomAbilitySystemComponent() const { return Cast<UCustomAbilitySystemComponent>(AbilitySystemComponent); }

void ALaserTank::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (CurrentHorizontalMovement != 0) {
		// We multiply by 60 because the Speed is in texels per frame
		CurrentTexelPosition.X += HorizontalVelocity * DeltaTime * 60.f;

		AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));

		CurrentTexelPosition.X = FMath::Clamp(CurrentTexelPosition.X, 0, GameState->GetMapSize().X - CharacterSize.X - 1);


		FIntPoint RoundedTexelPosition = FIntPoint(FMath::RoundToInt(CurrentTexelPosition.X), FMath::RoundToInt(CurrentTexelPosition.Y));
		FVector NewWorldPos = GameState->TexelToWorldPos(RoundedTexelPosition);
		RootComponent->SetWorldLocation(NewWorldPos);
	}
}

void ALaserTank::SetHorizontalMovement(float HorizontalMovement) {
	if (HorizontalMovement == CurrentHorizontalMovement) {
		return;
	}

	CurrentHorizontalMovement = HorizontalMovement;
	HorizontalVelocity = HorizontalMovement * Speed;
}

void ALaserTank::SetAbilityInput(EGASAbilityInput input, bool pressed) {}