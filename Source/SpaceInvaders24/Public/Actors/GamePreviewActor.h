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

	// TODO: Comentar esto
	UPROPERTY()
	FRotator GameObjectOrientation;

	// // TODO: Comentar esto
	// UPROPERTY()
	// float TexelToWorldFactorOfConversion;

	// // TODO: Comentar esto
	// UPROPERTY()
	// float WorldToTexelFactorOfConversion;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent *Plane;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UArrowComponent *RelativeForward;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UArrowComponent *RelativeUp;

	/**
	 * If your actor needs to access its voxels using coordinates, such as Bunker or the BlastTrail of a shot, you'll need
	 * to position this actor at the same position as the voxel at coordinate (0, 0). This actor transforms, based on its
	 * relative position, the relative position of each voxel to world coordinates and vice versa. Similarly, we need its
	 * sibling component, SceneInMaxRelativePos, to do the same but for voxels at coordinate (width - 1, height - 1) according
	 * to the size specified in ActorLocalBounds.Z and W.
	 *
	 * If your actor doesn't need to access its voxels by coordinate, ignore these two components.
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneInMinRelativePos;

	/**
	 * If your actor needs to access its voxels using coordinates, such as Bunker or the BlastTrail of a Shot, you'll need
	 * to position this actor at the same position as the voxel at coordinate (width - 1, height - 1) according to the size
	 * specified in ActorLocalBounds.Z and W. This actor transforms, based on its relative position, the relative position
	 * of each voxel to world coordinates and vice versa. Similarly, we need its sibling component, SceneInMinRelativePos,
	 * to do the same but for voxels at coordinate (0, 0).
	 *
	 * If your actor doesn't need to access its voxels by coordinate, ignore these two components.
	 */
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
	FIntPoint GetMapSize() const;

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

	// // TODO: Comentar esto
	// UFUNCTION(BlueprintCallable)
	// float GetTexelToWorldFactorOfConversion() const;

	// // TODO: Comentar esto
	// UFUNCTION(BlueprintCallable)
	// float GetWorldToTexelFactorOfConversion() const;
};
