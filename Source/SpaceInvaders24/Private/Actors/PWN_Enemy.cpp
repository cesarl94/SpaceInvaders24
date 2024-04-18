// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PWN_Enemy.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values
APWN_Enemy::APWN_Enemy() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APWN_Enemy::BeginPlay() { Super::BeginPlay(); }

// This function will be triggered in BP
void APWN_Enemy::Animate_Implementation(bool Forward, float Rate) const {}

// Called every frame
void APWN_Enemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (UKismetSystemLibrary::GetFrameCount() % 100 == 0) {
		if (UKismetSystemLibrary::GetFrameCount() % 200 == 0) {
			Animate(true, 2);
		} else {

			Animate(false, 2);
		}
	}
}
