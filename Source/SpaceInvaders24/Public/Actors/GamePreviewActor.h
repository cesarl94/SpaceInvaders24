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

/**
 * This class is responsible for transforming all world coordinates into texels and vice versa. It provides a preview of a plane
 * with a screenshot of the original game to position wherever we want in the world using that screenshot as a reference.
 */

UCLASS()
class SPACEINVADERS24_API AGamePreviewActor : public AActor {
	GENERATED_BODY()

private:
	// Cached value stored to facilitate calculations for position transformation. Takes the relative position of the SceneInMinRelativePos node.
	UPROPERTY()
	FVector MinRelativeLocation;

	// Cached value stored to facilitate calculations for position transformation. Takes the relative position of the SceneInMaxRelativePos node.
	UPROPERTY()
	FVector MaxRelativeLocation;

	// The orientation that all objects in the map will receive. Constructed collectively between the RelativeForward and RelativeUp components.
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

	// Returns the count of texels of the world. Width in X, Height in Y
	UFUNCTION(BlueprintCallable)
	FIntPoint GetMapSize() const;

	// It takes a 3D world position and returns the position in texels. The relative Z-axis is ignored.
	UFUNCTION(BlueprintCallable)
	FIntPoint WorldToTexelPos(FVector WorldPos) const;

	// It takes a position in texels and returns a 3D world position. The Z-axis is flattened.
	UFUNCTION(BlueprintCallable)
	FVector TexelToWorldPos(FIntPoint TexelPos) const;

	// It returns the forward direction of the game in 3D, meaning the direction in which the enemies are facing.
	UFUNCTION(BlueprintCallable)
	FVector GetForward() const;

	// It returns the up direction of the game in 3D, meaning the direction in which the enemies would look if they were to raise their view by 90°.
	UFUNCTION(BlueprintCallable)
	FVector GetUp() const;

	/**
	 * It returns the orientation that all objects in the game should have if they were part of the preview. This data is
	 * constructed collectively with the forward and up vectors (with forward taking priority).
	 */
	UFUNCTION(BlueprintCallable)
	FRotator GetGameObjectOrientation() const;
};
