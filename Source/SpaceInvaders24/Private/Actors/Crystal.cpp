
#include "Actors/Crystal.h"

#include "Actors/BlastTrail.h"
#include "Actors/Enemy.h"
#include "Core/GS_SpaceInvaders24.h"
#include "Kismet/GameplayStatics.h"
#include "Math/IntPoint.h"
#include "Math/Vector.h"


void ACrystal::SpawnBlastTrail() {
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

void ACrystal::ManualInitialize() {
	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));
	GameState->ReportNewCrystal(this);
}

void ACrystal::Kill(bool IsForcedKill) {
	SpawnBlastTrail();
	OnHit.Broadcast(this, !IsForcedKill);
	Destroy();
}
