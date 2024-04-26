// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Math/IntVector.h"
#include "Math/Vector4.h"
#include "Utils/Enums.h"

#include "ActorInTexels.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTouchLimitEvent, EDirection, Direction);

UCLASS()
class SPACEINVADERS24_API AActorInTexels : public APawn {
	GENERATED_BODY()

	UPROPERTY()
	FVector2D CurrentTexelPosition;

	UPROPERTY()
	FVector2D TexelVelocity;

protected:
	virtual void BeginPlay() override;

	// Components:

	// The new root component
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneComponent;

	// Put inside of this Scene all voxels and visible things
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *GraphicNodes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent *Collider;

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

	// A 2D box that covers the actor, in texels. Like a collider. Offset in X, offset in Y, width, height
	// IMPORTANT: This is a float-based vector to animate, but only integers values are allowed!!
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Actor In Texels", Interp)
	FVector4 ActorLocalBoundsFloat;

	// A 2D box that is the playable zone for this actor, in texels. Offset in X, offset in Y, width, height
	// CAUTION: If the width of the map is 224, and your character is 13 pixels width, stay calm and put 224, don't substract the width!
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Actor In Texels")
	FIntVector4 ActorLimits;

	// Avoids move the actor outside the actor limits, respect its ActorLocalBounds
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Actor In Texels")
	bool ClampPosition{false};


public:
	AActorInTexels();

	// Call this function to move an object that has velocity in texels. The delta time it receives can be either the normal delta time or the CrystalDeltaTime from the GameTimeManager
	virtual void ApplyVelocity(float DeltaTime);

	/**
	 * Returns the internal texel position of the object. While it is not the visible texel position, it is floating-point and
	 * includes intermediate positions between each texel. This texel position is affected by the TexelVelocity.
	 */
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Actor In Texels")
	virtual FVector2D GetFloatTexelPosition() const;

	// Returns the visible position of the object in texels
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Actor In Texels")
	virtual FIntPoint GetIntTexelPosition() const;

	/**
	 * This function may be more complex than it seems:
	 *
	 * Simply put, it sets the real position of the object. If the values are very similar to the current ones, no visible change may be reflected.
	 *
	 * Additionally, if the internal variable "ClampPosition" is activated, this function will automatically clamp the object's position to the allowed
	 * area, established collectively between "ActorLimits" and "ActorLocalBounds".
	 *
	 * It also triggers the "OnTouchLimit" event when objects touch the edges of the allowed area or exceed it (in case ClampPosition is not activated).
	 * These events are not constantly triggered if the actor is outside the area, but only when it wasn't touching that edge and now it is, providing
	 * the direction in which it tries to escape.
	 */
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Actor In Texels")
	virtual void SetTexelPosition(FVector2D NewTexelPosition, bool Sweep = false);

	// Gets the current velocity of the object in texels.
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Actor In Texels")
	virtual FVector2D GetTexelVelocity() const;

	/**
	 * Sets the current velocity of the object in texels.
	 *
	 * CAUTION: The velocity will not be applied automatically. For that, it is necessary to call ApplyVelocity each frame and provide the DeltaTime to use.
	 */
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Actor In Texels")
	virtual void SetTexelVelocity(FVector2D NewTexelVelocity);

	// Gets the visible bounding box of the object. Established collectively between the Texel Position and ActorLocalBounds.
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Actor In Texels")
	virtual FIntVector4 GetIntTexelBoundingBox() const;

	// Events:
	/**
	 * This event is triggered within the "SetTexelPosition" function when objects touch the edges of the allowed area or
	 * exceed it (in case ClampPosition is not activated). These events are not constantly triggered if the actor is outside
	 * the area, but only when it wasn't touching that edge and now it is, providing the direction in which it tries to escape.
	 */
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FOnTouchLimitEvent OnTouchLimit;
};
