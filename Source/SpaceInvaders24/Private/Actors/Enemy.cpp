// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy.h"

#include "Actors/BlastTrail.h"
#include "Actors/Bunker.h"
#include "Actors/Crystal.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SimpleVoxel.h"
#include "Core/GS_SpaceInvaders24.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "GAS/CustomAbilitySystemComponent.h"
#include "GAS/CustomAttributeSet.h"
#include "GAS/CustomGameplayAbility.h"
#include "GAS/GASEnums.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/IntPoint.h"
#include "Math/IntVector.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "Structs/BlastTrailData.h"
#include "Structs/GunData.h"


void AEnemy::InitializeGAS() {
	UCustomAbilitySystemComponent *ASC = GetCustomAbilitySystemComponent();
	if (ASC == nullptr) {
		return;
	}

	if (AttributeSet == nullptr) {
		UGameplayStatics::GetGameState(this);
		return;
	}

	ASC->SetAttributeSetReference(AttributeSet);
	ASC->InitAbilityActorInfo(this, this);
	ASC->OnAttributeChange.AddUniqueDynamic(this, &AEnemy::OnAttributeChanged);


	InitializeAbilities();

	BindInput();
}

void AEnemy::InitializeAbilities() {
	for (TSubclassOf<UCustomGameplayAbility> &Ability : DefaultAbilities) {
		const EGASAbilityInput AbilityInputID = Ability.GetDefaultObject()->AbilityInputID;

		if (AbilityInputID == EGASAbilityInput::None) {
			UKismetSystemLibrary::PrintString(GetWorld(), "You're trying to set a Default Ability with \"None\" as Ability Input ID. This ability was canceled.", true, true, FColor::Red, 5);
		} else {
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(AbilityInputID), this));
		}
	}
}

void AEnemy::ApplyDefaultEffects() {
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> &Effect : DefaultEffects) {
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (SpecHandle.IsValid()) {
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AEnemy::BindInput() {
	if (bIsInputBound || !AbilitySystemComponent || !IsValid(InputComponent)) {
		return;
	}

	FTopLevelAssetPath EnumAssetPath = FTopLevelAssetPath(FName("/Script/SpaceInvaders24"), FName("EGASAbilityInput"));
	AbilitySystemComponent->BindAbilityActivationToInputComponent(
		InputComponent, FGameplayAbilityInputBinds(FString("Confirm"), FString("Cancel"), EnumAssetPath, static_cast<int32>(EGASAbilityInput::Confirm), static_cast<int32>(EGASAbilityInput::Cancel)));

	bIsInputBound = true;
}

void AEnemy::OnAttributeChanged(EPlayerAttribute AttributeEnum, float OldValue, float NewValue) {
	if (AttributeEnum == EPlayerAttribute::Health && FMath::RoundToInt(NewValue) == 0) {
		Kill(false);
	}
}

void AEnemy::OnBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult) {
	if (ABunker *HitBunker = Cast<ABunker>(OtherActor)) {
		OverlappedBunker = HitBunker;
	}
}


void AEnemy::OnBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {
	if (OtherActor == OverlappedBunker) {
		OverlappedBunker = nullptr;
	}
}

void AEnemy::SpawnBlastTrail() {
	if (BlastTrailData.BlastTrailActorClass == nullptr) {
		return;
	}

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));

	FIntPoint TexelIntPoint = GetIntTexelPosition() + BlastTrailData.Offset;
	FVector BlastTrailWorldPosition = GameState->TexelToWorldPos(TexelIntPoint);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABlastTrail *BlastTrail = GetWorld()->SpawnActor<ABlastTrail>(BlastTrailData.BlastTrailActorClass, BlastTrailWorldPosition, GameState->GetGameObjectOrientation(), ActorSpawnParams);
	BlastTrail->ManualInitialize(BlastTrailData.Duration);
}

void AEnemy::SpawnCrystal() {
	if (CrystalDropData.CrystalDroppedClass == nullptr) {
		return;
	}

	int32 RandomNum = FMath::RandRange(1, 100);
	if (RandomNum > CrystalDropData.CrystalDropProbability) {
		return;
	}

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));

	FIntPoint TexelIntPoint = GetIntTexelPosition() + CrystalDropData.CrystalOffset;
	FVector CrystalWorldPosition = GameState->TexelToWorldPos(TexelIntPoint);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACrystal *Crystal = GetWorld()->SpawnActor<ACrystal>(CrystalDropData.CrystalDroppedClass, CrystalWorldPosition, GameState->GetGameObjectOrientation(), ActorSpawnParams);
	Crystal->ManualInitialize();
}

// This function will be triggered in BP
void AEnemy::Animate_Implementation(bool Forward, float Rate) const {}

AEnemy::AEnemy() : Super() {
	// GAS Components:
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>("Ability System Component");
	AttributeSet = CreateDefaultSubobject<UCustomAttributeSet>("Attribute Set");
}

void AEnemy::ManualInitialize(FIntPoint CoordinateInGrid) {
	EnemyCoordinateInGrid = CoordinateInGrid;

	SetVisibility(false);

	Collider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AEnemy::OnBoxBeginOverlap);
	Collider->OnComponentEndOverlap.AddUniqueDynamic(this, &AEnemy::OnBoxEndOverlap);

	InitializeGAS();
}

void AEnemy::ManualTick() {
	if (OverlappedBunker == nullptr) {
		return;
	}

	// TODO: comentar esto

	FIntVector4 EnemyBounds = GetIntTexelBoundingBox();
	FIntVector4 BunkerBounds = OverlappedBunker->GetIntTexelBoundingBox();

	if (EnemyBounds.X + EnemyBounds.Z <= BunkerBounds.X || BunkerBounds.X + BunkerBounds.Z <= EnemyBounds.X || EnemyBounds.Y + EnemyBounds.W <= BunkerBounds.Y ||
		BunkerBounds.Y + BunkerBounds.W <= EnemyBounds.Y) {
		// No overlap
		return;
	}

	// Calculate the coordinates of the new bounding box representing the overlapping area
	int32 NewLeft = FMath::Max(EnemyBounds.X, BunkerBounds.X);
	int32 NewTop = FMath::Max(EnemyBounds.Y, BunkerBounds.Y);
	int32 NewRight = FMath::Min(EnemyBounds.X + EnemyBounds.Z, BunkerBounds.X + BunkerBounds.Z);
	int32 NewBottom = FMath::Min(EnemyBounds.Y + EnemyBounds.W, BunkerBounds.Y + BunkerBounds.W);
	int32 NewWidth = NewRight - NewLeft;
	int32 NewHeight = NewBottom - NewTop;

	// Create the new overlapping bounding box
	FIntVector4 OverlappingBounds = FIntVector4(NewLeft, NewTop, NewWidth, NewHeight);
	FIntPoint BunkerIntTexelPosition = OverlappedBunker->GetIntTexelPosition();

	for (int32 x = 0; x < OverlappingBounds.Z; x++) {
		for (int32 y = 0; y < OverlappingBounds.W; y++) {
			int32 BunkerLocalVoxelX = OverlappingBounds.X + x - BunkerIntTexelPosition.X;
			int32 BunkerLocalVoxelY = OverlappingBounds.Y + y - BunkerIntTexelPosition.Y;
			USimpleVoxel *BunkerBrick = OverlappedBunker->GetBrickInCoordinate(BunkerLocalVoxelX, BunkerLocalVoxelY);
			if (BunkerBrick != nullptr && BunkerBrick->IsEnabled()) {
				BunkerBrick->SetEnabled(false);
			}
		}
	}
}

void AEnemy::TriggerMoveAnimation(float PlayRate) {
	Animate(!LastAnimationWasForward, 2 * PlayRate);
	LastAnimationWasForward = !LastAnimationWasForward;
}

void AEnemy::ManualReset(FIntPoint NewTexelPosition) {
	SetTexelPosition(FVector2D(NewTexelPosition.X, NewTexelPosition.Y));
	LastAnimationWasForward = false;
	// we need to set to the first frame inmediatly
	Animate(false, 1000000.f);

	ApplyDefaultEffects();

	SetVisibility(true);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Alive = true;
}

FIntPoint AEnemy::GetCoordinateInEnemyGrid() const { return EnemyCoordinateInGrid; }

FGunData AEnemy::GetGunData() const { return GunData; }

bool AEnemy::IsAlive() const { return Alive; }

void AEnemy::Kill(bool IsForcedKill) {
	Alive = false;
	SetVisibility(false);

	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	int32 PointsGiven = 0;

	if (!IsForcedKill) {
		SpawnBlastTrail();
		SpawnCrystal();

		int32 PointsGivenRandomId = FMath::RandRange(0, PointsThatCouldGive.Num() - 1);
		PointsGiven = PointsThatCouldGive[PointsGivenRandomId];
	}

	OnDie.Broadcast(this, PointsGiven);
}

UAbilitySystemComponent *AEnemy::GetAbilitySystemComponent() const { return AbilitySystemComponent; }

UCustomAbilitySystemComponent *AEnemy::GetCustomAbilitySystemComponent() const { return Cast<UCustomAbilitySystemComponent>(AbilitySystemComponent); }

void AEnemy::SetAbilityInput(EGASAbilityInput input, bool pressed) {
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