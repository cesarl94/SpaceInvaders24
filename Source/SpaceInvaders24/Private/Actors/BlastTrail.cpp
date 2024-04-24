#include "Actors/BlastTrail.h"

#include "Components/BoxComponent.h"


ABlastTrail::ABlastTrail() {
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SceneComponent->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Collider->SetupAttachment(SceneComponent);

	GraphicNodes = CreateDefaultSubobject<USceneComponent>(TEXT("Graphic Nodes"));
	GraphicNodes->SetupAttachment(SceneComponent);
}

void ABlastTrail::ManualInitialize(float Duration) {
	GetWorld()->GetTimerManager().SetTimer(
		InputTimeHandle, [&]() { Destroy(); }, Duration, false);
}