// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/IntPoint.h"
#include "Utils/Enums.h"

#include "PWN_Enemy.generated.h"


UCLASS()
class SPACEINVADERS24_API APWN_Enemy : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APWN_Enemy();

private:
	UPROPERTY()
	EEnemyState State{EEnemyState::IDLE};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Stats")
	EEnemyType Type;

	// Width in X, Height in Y
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Stats")
	FIntPoint EnemySize;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemy Stats")
	int32 Health;

	UFUNCTION(BlueprintNativeEvent, Category = "SpaceInvaders24: Native Events")
	void Animate(bool Forward, float Rate) const;

	UFUNCTION(BlueprintNativeEvent, Category = "SpaceInvaders24: Native Events")
	void DieAnimation(bool Forward, float Rate) const;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
