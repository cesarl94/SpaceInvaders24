// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PWN_Enemy.generated.h"

UCLASS()
class SPACEINVADERS24_API APWN_Enemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APWN_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintNativeEvent, Category = "Native Events")
	void Animate(bool Forward, float Rate) const;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
