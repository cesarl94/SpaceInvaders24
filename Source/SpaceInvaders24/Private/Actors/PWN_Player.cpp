

#include "Actors/PWN_Player.h"

APWN_Player::APWN_Player() {
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SceneComponent->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Collider->SetupAttachment(SceneComponent);

	GraphicNodes = CreateDefaultSubobject<USceneComponent>(TEXT("Graphic Nodes"));
	GraphicNodes->SetupAttachment(SceneComponent);
}

void APWN_Player::BeginPlay() { Super::BeginPlay(); }

void APWN_Player::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void APWN_Player::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) { Super::SetupPlayerInputComponent(PlayerInputComponent); }
