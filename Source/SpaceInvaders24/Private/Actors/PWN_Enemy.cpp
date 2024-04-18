// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PWN_Enemy.h"

// Sets default values
APWN_Enemy::APWN_Enemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APWN_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void APWN_Enemy::Animate_Implementation(bool Forward) const{
	
}

// Called every frame
void APWN_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APWN_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

