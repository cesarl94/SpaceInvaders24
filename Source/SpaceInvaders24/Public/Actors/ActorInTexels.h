// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Math/IntVector.h"
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

	// A 2D box that covers the actor, in texels. Like a collider. Offset in X, offset in Y, width, height
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Actor In Texels")
	FIntVector4 ActorLocalBounds;

	// A 2D box that is the playable zone for this actor, in texels. Offset in X, offset in Y, width, height
	// CAUTION: If the width of the map is 224, and your character is 13 pixels width, stay calm and put 224, don't substract the width!
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Actor In Texels")
	FIntVector4 ActorLimits;

	// Avoids move the actor outside the actor limits, respect its ActorLocalBounds
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Actor In Texels")
	bool ClampPosition{false};


public:
	// TODO: comentar esto
	virtual void ApplyVelocity(float DeltaTime);

	// TODO: comentar esto
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Actor In Texels")
	virtual FVector2D GetFloatTexelPosition() const;

	// TODO: comentar esto. La posición visible del actor
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Actor In Texels")
	virtual FIntPoint GetIntTexelPosition() const;

	// TODO: comentar esto
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Actor In Texels")
	virtual void SetTexelPosition(FVector2D NewTexelPosition, bool Sweep = false);

	// TODO: comentar esto
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Actor In Texels")
	virtual FVector2D GetTexelVelocity() const;

	// TODO: comentar esto
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Actor In Texels")
	virtual void SetTexelVelocity(FVector2D NewTexelVelocity);

	// TODO: comentar esto
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Actor In Texels")
	virtual FIntVector4 GetIntTexelBoundingBox() const;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FOnTouchLimitEvent OnTouchLimit;
};
