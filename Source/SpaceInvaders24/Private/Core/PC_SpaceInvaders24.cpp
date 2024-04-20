
#include "Core/PC_SpaceInvaders24.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"


void APC_SpaceInvaders24::BeginPlay() {
	Super::BeginPlay();

	ACameraActor *CameraActor = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(this, ACameraActor::StaticClass()));

	SetViewTargetWithBlend(CameraActor);
}