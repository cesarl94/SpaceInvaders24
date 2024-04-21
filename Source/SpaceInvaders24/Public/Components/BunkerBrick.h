// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "Utils/Enums.h"

#include "BunkerBrick.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEINVADERS24_API UBunkerBrick : public UStaticMeshComponent {
	GENERATED_BODY()

private:
	UPROPERTY()
	bool Enabled{true};

public:
	UBunkerBrick();

	UFUNCTION()
	void SetEnabled(bool NewEnabled);
};
