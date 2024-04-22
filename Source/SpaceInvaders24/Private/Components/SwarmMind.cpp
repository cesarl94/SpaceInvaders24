// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SwarmMind.h"

#include "Core/GS_SpaceInvaders24.h"


// Sets default values for this component's properties
USwarmMind::USwarmMind() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USwarmMind::BeginPlay() {
	Super::BeginPlay();

	// ...
}

void USwarmMind::ManualTick(float DeltaTime) { AGS_SpaceInvaders24 *GameState = GetOwner<AGS_SpaceInvaders24>(); }