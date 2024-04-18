// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, DisplayName = "Enemy Type")
enum class EEnemyType : uint8 {
	OCTOPUS UMETA(DisplayName = "Octopus"),
	CRAB UMETA(DisplayName = "Crab"),
	SQUID UMETA(DisplayName = "Squid"),
};

UENUM(BlueprintType, DisplayName = "Enemy Type")
enum class EEnemyState : uint8 {
	IDLE UMETA(DisplayName = "Idle"),
	MOVING UMETA(DisplayName = "Moving"),
	DYING UMETA(DisplayName = "Dying"),
	DEAD UMETA(DisplayName = "Dead"),
};
