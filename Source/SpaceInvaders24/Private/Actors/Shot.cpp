// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Shot.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Actors/BlastTrail.h"
#include "Actors/Bunker.h"
#include "Actors/Crystal.h"
#include "Actors/Enemy.h"
#include "Actors/LaserTank.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/GS_SpaceInvaders24.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/IntPoint.h"
#include "Math/Vector.h"
#include "Structs/BlastTrailData.h"
#include "Utils/Enums.h"


void AShot::OnBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult) {

	if (GetType() == EShotType::SIMPLE_LINE) {
		if (ACrystal *HitCrystal = Cast<ACrystal>(OtherActor)) {
			HitCrystal->Kill(false);
		}

		if (AEnemy *HitEnemy = Cast<AEnemy>(OtherActor)) {
			// I need to disable collision before kill enemy because if the enemy drop any crystal, I could collide it also before call Dissapear()
			Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			IAbilitySystemInterface *ActorInstigator = Cast<IAbilitySystemInterface>(GetInstigator());
			ActorInstigator->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), HitEnemy->GetAbilitySystemComponent());
			Dissapear();
			return;
		}

		if (AShot *HitShot = Cast<AShot>(OtherActor)) {
			if (HitShot->GetType() != EShotType::SIMPLE_LINE) {
				Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				OverlappedComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				HitShot->Dissapear();
				Dissapear();
				return;
			}
		}


	} else {
		if (ALaserTank *HitPlayer = Cast<ALaserTank>(OtherActor)) {
			Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			IAbilitySystemInterface *ActorInstigator = Cast<IAbilitySystemInterface>(GetInstigator());
			ActorInstigator->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), HitPlayer->GetAbilitySystemComponent());
			Dissapear();
			return;
		}
	}

	if (ABunker *HitBunker = Cast<ABunker>(OtherActor)) {
		OverlappedBunker = HitBunker;
	}
}


void AShot::OnBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {
	if (OtherActor == OverlappedBunker) {
		OverlappedBunker = nullptr;
	}
}

bool AShot::IsTouchingBunkerBrick() {
	if (OverlappedBunker == nullptr) {
		return false;
	}

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));

	for (int32 i = 0; i < Voxels.Num(); i++) {
		UStaticMeshComponent *Voxel = Voxels[i];
		FVector VoxelWorldPosition = Voxel->GetComponentLocation();

		FIntPoint VoxelTexelPos = GameState->WorldToTexelPos(VoxelWorldPosition);
		FIntVector4 BunkerBounds = OverlappedBunker->GetIntTexelBoundingBox();

		if (VoxelTexelPos.X < BunkerBounds.X || VoxelTexelPos.Y < BunkerBounds.Y || VoxelTexelPos.X >= BunkerBounds.X + BunkerBounds.Z || VoxelTexelPos.Y >= BunkerBounds.Y + BunkerBounds.W) {
			continue;
		}

		FIntPoint VoxelInBunkerLocalCoordinates = FIntPoint(VoxelTexelPos.X - BunkerBounds.X, VoxelTexelPos.Y - BunkerBounds.Y);

		USimpleVoxel *BunkerBrick = OverlappedBunker->GetBrickInCoordinate(VoxelInBunkerLocalCoordinates.X, VoxelInBunkerLocalCoordinates.Y);

		if (BunkerBrick != nullptr && BunkerBrick->IsEnabled()) {
			return true;
		}
	}

	return false;
}

void AShot::TouchLimit(EDirection Direction) { Dissapear(); }

void AShot::Dissapear() {
	if (BlastTrailData.BlastTrailActorClass != nullptr) {
		AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FIntPoint TexelIntPoint = GetIntTexelPosition() + BlastTrailData.Offset;
		FVector BlastTrailWorldPosition = GameState->TexelToWorldPos(TexelIntPoint);

		ABlastTrail *BlastTrail = GetWorld()->SpawnActor<ABlastTrail>(BlastTrailData.BlastTrailActorClass, BlastTrailWorldPosition, GameState->GetGameObjectOrientation(), ActorSpawnParams);
		BlastTrail->ManualInitialize(BlastTrailData.Duration);
	}

	OnHit.Broadcast(this);
	Destroy();
}

void AShot::BeginPlay() {
	Super::BeginPlay();

	GetComponents<UStaticMeshComponent>(Voxels);

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));
	GameState->ReportNewShot(this);

	OnTouchLimit.AddUniqueDynamic(this, &AShot::TouchLimit);
}

AShot::AShot() : Super() {
	Collider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AShot::OnBoxBeginOverlap);
	Collider->OnComponentEndOverlap.AddUniqueDynamic(this, &AShot::OnBoxEndOverlap);
}

void AShot::ManualTick(float DeltaTime) {
	if (IsTouchingBunkerBrick()) {
		Dissapear();
		return;
	}

	// TODO: comentar esto

	FVector2D PrevTexelPosition = GetFloatTexelPosition();
	FVector2D NewTexelPosition = PrevTexelPosition + GetTexelVelocity() * DeltaTime;

	FIntPoint PrevIntTexelPosition = FIntPoint(FMath::RoundToInt(PrevTexelPosition.X), FMath::RoundToInt(PrevTexelPosition.Y));
	FIntPoint NewIntTexelPosition = FIntPoint(FMath::RoundToInt(NewTexelPosition.X), FMath::RoundToInt(NewTexelPosition.Y));

	int32 DifferenceY = NewIntTexelPosition.Y - PrevIntTexelPosition.Y;

	if (DifferenceY == 0) {
		SetTexelPosition(NewTexelPosition, false);
		return;
	}

	int32 DirectionY = DifferenceY > 0 ? 1 : -1;

	for (int32 i = 1, limit = FMath::Abs(DifferenceY); i < limit; i++) {
		FIntPoint NewIntermediatePosition = PrevIntTexelPosition + FIntPoint(0, DirectionY * i);
		SetTexelPosition(NewIntermediatePosition, false);

		if (IsTouchingBunkerBrick()) {
			Dissapear();
			return;
		}
	}

	SetTexelPosition(NewTexelPosition, false);
}

float AShot::GetMovementSpeed() const { return MovementSpeed; }

EShotType AShot::GetType() const { return Type; }