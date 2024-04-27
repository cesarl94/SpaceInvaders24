#include "Actors/BlastTrail.h"

#include "Actors/Bunker.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SimpleVoxel.h"
#include "Components/StaticMeshComponent.h"
#include "Core/GS_SpaceInvaders24.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Math/IntVector.h"
#include "Math/Vector.h"


void ABlastTrail::OnBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult) {
	if (ABunker *HitBunker = Cast<ABunker>(OtherActor)) {
		// I need to disable collision before kill enemy because if the enemy drop any crystal, I could collide it also before call Dissapear()
		Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		ErodeBunker(HitBunker);
	}
}

void ABlastTrail::ErodeBunker(ABunker *Bunker) {
	TArray<UStaticMeshComponent *> Voxels;
	GetComponents<UStaticMeshComponent>(Voxels);

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));

	for (int32 i = 0; i < Voxels.Num(); i++) {
		UStaticMeshComponent *Voxel = Voxels[i];
		FVector VoxelWorldPosition = Voxel->GetComponentLocation();

		FIntPoint VoxelTexelPos = GameState->WorldToTexelPos(VoxelWorldPosition);
		FIntVector4 BunkerBounds = Bunker->GetIntTexelBoundingBox();

		if (VoxelTexelPos.X < BunkerBounds.X || VoxelTexelPos.Y < BunkerBounds.Y || VoxelTexelPos.X >= BunkerBounds.X + BunkerBounds.Z || VoxelTexelPos.Y >= BunkerBounds.Y + BunkerBounds.W) {
			continue;
		}
		FIntPoint VoxelInBunkerLocalCoordinates = FIntPoint(VoxelTexelPos.X - BunkerBounds.X, VoxelTexelPos.Y - BunkerBounds.Y);

		USimpleVoxel *BunkerBrick = Bunker->GetBrickInCoordinate(VoxelInBunkerLocalCoordinates.X, VoxelInBunkerLocalCoordinates.Y);

		if (BunkerBrick != nullptr && BunkerBrick->IsEnabled()) {
			BunkerBrick->SetEnabled(false);
		}
	}
}

ABlastTrail::ABlastTrail() : Super() { Collider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABlastTrail::OnBoxBeginOverlap); }

void ABlastTrail::ManualInitialize(float Duration) {

	GetWorld()->GetTimerManager().SetTimer(
		InputTimeHandle, [&]() { Destroy(); }, Duration, false);
}