// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "Structs/GunData.h"
#include "Utils/Enums.h"

#include "LaserTank.generated.h"


UCLASS()
class SPACEINVADERS24_API ALaserTank : public APawn {
	GENERATED_BODY()

private:
	UPROPERTY()
	FVector2D CurrentTexelPosition;

	UPROPERTY()
	float HorizontalVelocity;

	UPROPERTY()
	float CurrentHorizontalMovement{0};

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *GraphicNodes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent *Collider;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Laser Tank Data")
	FIntPoint CharacterSize;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Laser Tank Data")
	FGunData GunData;

	// texels per frame
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Laser Tank Data")
	float Speed{1};

public:
	ALaserTank();

	// Used to initialize
	virtual void PossessedBy(AController *NewController) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, DisplayName = "LaserTank SetHorizontalMovement")
	void SetHorizontalMovement(float HorizontalMovement);
};
