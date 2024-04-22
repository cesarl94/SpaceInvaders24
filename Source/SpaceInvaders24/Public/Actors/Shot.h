// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/ActorInTexels.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Math/IntVector.h"
#include "Math/Vector2D.h"
#include "Utils/Enums.h"

#include "Shot.generated.h"


UCLASS()
class SPACEINVADERS24_API AShot : public AActorInTexels {
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	// Components:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *GraphicNodes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent *Collider;

	// Serialized data:
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Shot Stats")
	EShotType Type;

	// texels per frame
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Shot Stats")
	float MovementSpeed{2};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: Shot Stats")
	float AnimationPlayRate{2.5};

public:
	AShot();

	virtual void Tick(float DeltaTime) override;
};
