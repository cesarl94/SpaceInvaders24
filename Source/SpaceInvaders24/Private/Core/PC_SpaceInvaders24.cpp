
#include "Core/PC_SpaceInvaders24.h"

#include "Camera/CameraActor.h"
#include "Core/GS_SpaceInvaders24.h"
#include "Kismet/GameplayStatics.h"


void APC_SpaceInvaders24::BeginPlay() {
	Super::BeginPlay();

	ACameraActor *CameraActor = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(this, ACameraActor::StaticClass()));

	SetViewTargetWithBlend(CameraActor);

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));
	GameState->OnPlayerControllerConnected(this);
}