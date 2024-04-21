// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/IntVector.h"
#include "Utils/Enums.h"

#include "Enemy.generated.h"


UCLASS()
class SPACEINVADERS24_API AEnemy : public AActor {
	GENERATED_BODY()

public:
	AEnemy();

private:
	UPROPERTY()
	EEnemyState State{EEnemyState::IDLE};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *GraphicNodes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent *Collider;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Stats")
	EEnemyType Type;

	// Offset in X, offset in Y, width, height
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Stats")
	FIntVector4 EnemyLocalBounds;

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
