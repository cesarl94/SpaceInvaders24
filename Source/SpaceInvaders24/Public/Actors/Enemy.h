// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/ActorInTexels.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Math/IntPoint.h"
#include "Math/IntVector.h"
#include "Math/Vector2D.h"
#include "Utils/Enums.h"

#include "Enemy.generated.h"


class AEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDie, AEnemy *, DiedEnemy);

UCLASS()
class SPACEINVADERS24_API AEnemy : public AActorInTexels {
	GENERATED_BODY()


private:
	UPROPERTY()
	EEnemyState State{EEnemyState::IDLE};

	UPROPERTY()
	FIntPoint EnemyCoordinateInGrid;

	UPROPERTY()
	bool Alive{false};


protected:
	// Components:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *GraphicNodes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent *Collider;

	// Serialized data:
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Stats")
	EEnemyType Type;

	// The amount of points that this enemy could give you will be taked randomly inside of this array.
	// E.g: A simple enemy should have only 1 element. UFO could have 3.
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Stats")
	TArray<int32> PointsThatCouldGive;

	// In percentage
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Stats")
	int32 CrystalDropProbability;

	UFUNCTION(BlueprintNativeEvent, Category = "SpaceInvaders24: Native Events")
	void Animate(bool Forward, float Rate) const;

	UFUNCTION(BlueprintNativeEvent, Category = "SpaceInvaders24: Native Events")
	void DieAnimation(bool Forward, float Rate) const;


public:
	AEnemy();

	// Called from GS_SpaceInvaders24 after create
	void ManualInitialize(FIntPoint CoordinateInGrid);

	UFUNCTION()
	void ManualReset(FIntPoint NewTexelPosition);

	UFUNCTION()
	FIntPoint GetEnemyCoordinateInGrid() const;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, DisplayName = "Enemy IsAlive")
	bool IsAlive() const;

	UFUNCTION()
	void Die();

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FOnEnemyDie OnDie;
};
