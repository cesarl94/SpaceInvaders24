// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, DisplayName = "Enemy Type")
enum class EEnemyType : uint8 {
	OCTOPUS UMETA(DisplayName = "Octopus"),
	CRAB UMETA(DisplayName = "Crab"),
	SQUID UMETA(DisplayName = "Squid"),
};

UENUM(BlueprintType, DisplayName = "Enemy State")
enum class EEnemyState : uint8 {
	IDLE UMETA(DisplayName = "Idle"),
	MOVING UMETA(DisplayName = "Moving"),
	DYING UMETA(DisplayName = "Dying"),
	DEAD UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType, DisplayName = "Time State")
enum class ETimeState : uint8 {
	// When game is showing you some information
	IDLE UMETA(DisplayName = "Idle"),
	// Common game flow
	FORWARD UMETA(DisplayName = "Forward"),
	// Crystal 1
	SLOW UMETA(DisplayName = "Slow"),
	// Crystal 2
	PAUSED UMETA(DisplayName = "Paused"),
	// Crystal 3
	BACKWARD UMETA(DisplayName = "Backward"),
};

UENUM(BlueprintType, DisplayName = "Map Bound Type")
enum class EMapBoundType : uint8 {
	SHOTS_BOUND UMETA(DisplayName = "Shots Bound"),
	ENEMY_BOUND UMETA(DisplayName = "Enemy Bound"),
	PLAYER_BOUND UMETA(DisplayName = "Player Bound"),
	UFO_BOUND UMETA(DisplayName = "UFO Bound"),
};