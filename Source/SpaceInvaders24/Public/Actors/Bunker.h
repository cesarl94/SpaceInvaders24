// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BunkerBrick.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/IntPoint.h"

#include "Bunker.generated.h"


UCLASS()
class SPACEINVADERS24_API ABunker : public AActor {
	GENERATED_BODY()

private:
	// CAUTION: Ultra-private variable
	// Since Unreal doesn't allow 2-dimensional-arrays, we'll use a 1D array indexed :(
	// Use with the functions: "GetBrickInCoordinate" and "SetBrickInCoordinate"
	// CAUTION2: Double reference array togheter with the another array of bricks, "TArray<UBunkerBrick *> Bricks".
	UPROPERTY()
	TArray<UBunkerBrick *> _BricksGrid;

	// TODO: Comentar
	UPROPERTY()
	TArray<UBunkerBrick *> Bricks;

	// TODO: Comentar esto
	UPROPERTY()
	FVector MinRelativeLocation;

	// TODO: Comentar esto
	UPROPERTY()
	FVector MaxRelativeLocation;

	// TODO: comentar que hace esto
	UFUNCTION()
	void SetBrickInCoordinate(int32 RelativeX, int32 RelativeY, UBunkerBrick *Brick);

	// TODO: comentar que hace esto
	UFUNCTION()
	FIntPoint Relative3DToRelativeTexelPos(FVector Relative3DPos) const;

	// TODO: comentar que hace esto
	UFUNCTION()
	FVector RelativeTexelToRelative3DPos(FIntPoint RelativeTexelPos) const;


protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *BricksScene;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent *Collider;

	// TODO: comentar que es esto
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneInMinRelativePos;

	// TODO: comentar que es esto
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneInMaxRelativePos;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Bunker Data")
	FIntPoint BunkerSize;


public:
	ABunker();

	// Called from GameState's BeginPlay
	void ManualInitialize();

	// TODO: comentar que hace esto
	UFUNCTION()
	const UBunkerBrick *GetBrickInCoordinate(int32 RelativeX, int32 RelativeY) const;

	// Returns all the bricks. CAUTION: Enabled and disabled bricks.
	UFUNCTION()
	const TArray<UBunkerBrick *> &GetAllBricks() const;
};
