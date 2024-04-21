// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Math/IntPoint.h"
#include "Structs/GunData.h"
#include "Utils/Enums.h"

#include "PWN_Player.generated.h"


UCLASS()
class SPACEINVADERS24_API APWN_Player : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APWN_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *GraphicNodes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent *Collider;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Player Data")
	FIntPoint CharacterSize;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Player Data")
	FGunData GunData;

	// texels per frame
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Player Data")
	float Velocity{1};

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
};
