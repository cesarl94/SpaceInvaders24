// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "Utils/Enums.h"

#include "SimpleVoxel.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEINVADERS24_API USimpleVoxel : public UStaticMeshComponent {
	GENERATED_BODY()

private:
	UPROPERTY()
	bool Enabled{true};

public:
	USimpleVoxel();

	UFUNCTION()
	void SetEnabled(bool NewEnabled);

	UFUNCTION()
	bool IsEnabled() const;
};
