// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/ActorInTexels.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Math/IntVector.h"
#include "Math/Vector2D.h"
#include "Utils/Enums.h"

#include "Enemy.generated.h"


UCLASS()
class SPACEINVADERS24_API AEnemy : public AActorInTexels {
	GENERATED_BODY()


private:
	UPROPERTY()
	EEnemyState State{EEnemyState::IDLE};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Stats")
	int32 Health;

	// In percentage
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Stats")
	int32 CrystalDropProbability;

	UFUNCTION(BlueprintNativeEvent, Category = "SpaceInvaders24: Native Events")
	void Animate(bool Forward, float Rate) const;

	UFUNCTION(BlueprintNativeEvent, Category = "SpaceInvaders24: Native Events")
	void DieAnimation(bool Forward, float Rate) const;


public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
};
