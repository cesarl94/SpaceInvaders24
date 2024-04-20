// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

#include "GamePreviewActor.generated.h"


UCLASS()
class SPACEINVADERS24_API AGamePreviewActor : public AActor {
	GENERATED_BODY()

private:
	UPROPERTY()
	FVector MinRelativeLocation;

	UPROPERTY()
	FVector MaxRelativeLocation;

	UPROPERTY()
	FRotator GameObjectOrientation;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent *Plane;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UArrowComponent *RelativeForward;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UArrowComponent *RelativeUp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneInMinRelativePos;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneInMaxRelativePos;

	// Count of texels of the world. Width in X, Height in Y
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: World Stats")
	FIntPoint MapSize;

public:
	AGamePreviewActor();

	// Called from GameState's BeginPlay
	void ManualInitialize();

	UFUNCTION(BlueprintCallable)
	FIntPoint WorldToTexelPos(FVector WorldPos);

	UFUNCTION(BlueprintCallable)
	FVector TexelToWorldPos(FIntPoint TexelPos);

	UFUNCTION(BlueprintCallable)
	FVector GetForward();

	UFUNCTION(BlueprintCallable)
	FVector GetUp();

	UFUNCTION(BlueprintCallable)
	FRotator GetGameObjectOrientation();
};
