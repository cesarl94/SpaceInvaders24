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

// TODO: comentar que es esta clase

UCLASS()
class SPACEINVADERS24_API AGamePreviewActor : public AActor {
	GENERATED_BODY()

private:
	// TODO: Comentar esto
	UPROPERTY()
	FVector MinRelativeLocation;

	// TODO: Comentar esto
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

	// TODO: comentar que es esto
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneInMinRelativePos;

	// TODO: comentar que es esto
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneInMaxRelativePos;

	// Count of texels of the world. Width in X, Height in Y
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: World Stats")
	FIntPoint MapSize;

public:
	AGamePreviewActor();

	// Called from GameState's BeginPlay
	void ManualInitialize();

	// TODO: comentar que hace esto
	UFUNCTION(BlueprintCallable)
	FIntPoint WorldToTexelPos(FVector WorldPos) const;

	// TODO: comentar que hace esto
	UFUNCTION(BlueprintCallable)
	FVector TexelToWorldPos(FIntPoint TexelPos) const;

	// TODO: comentar que hace esto
	UFUNCTION(BlueprintCallable)
	FVector GetForward() const;

	// TODO: comentar que hace esto
	UFUNCTION(BlueprintCallable)
	FVector GetUp() const;

	// TODO: comentar que hace esto
	UFUNCTION(BlueprintCallable)
	FRotator GetGameObjectOrientation() const;
};
