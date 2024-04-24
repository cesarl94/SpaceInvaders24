#include "Actors/BlastTrail.h"

#include "Components/BoxComponent.h"

void ABlastTrail::ManualInitialize(float Duration) {
	GetWorld()->GetTimerManager().SetTimer(
		InputTimeHandle, [&]() { Destroy(); }, Duration, false);
}