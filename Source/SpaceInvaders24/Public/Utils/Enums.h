// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, DisplayName = "Enemy Type")
enum class EEnemyType : uint8 {
	OCTOPUS UMETA(DisplayName = "Octopus"),
	CRAB UMETA(DisplayName = "Crab"),
	SQUID UMETA(DisplayName = "Squid"),
	UFO UMETA(DisplayName = "UFO"),
};

UENUM(BlueprintType, DisplayName = "Enemy State")
enum class EEnemyState : uint8 {
	IDLE UMETA(DisplayName = "Idle"),
	MOVING UMETA(DisplayName = "Moving"),
	DYING UMETA(DisplayName = "Dying"),
	DEAD UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType, DisplayName = "Shot Type")
enum class EShotType : uint8 {
	SIMPLE_LINE UMETA(DisplayName = "Simple Line"),
	ENEMY_A UMETA(DisplayName = "Enemy A"),
	ENEMY_B UMETA(DisplayName = "Enemy B"),
	ENEMY_C UMETA(DisplayName = "Enemy C"),
};

UENUM(BlueprintType, DisplayName = "Crystal Type")
enum class ECrystalType : uint8 {
	CRYSTAL_A UMETA(DisplayName = "Crystal A"),
	CRYSTAL_B UMETA(DisplayName = "Crystal B"),
	CRYSTAL_C UMETA(DisplayName = "Crystal C"),
};

UENUM(BlueprintType, DisplayName = "")
enum class EDirection : uint8 { UP, DOWN, LEFT, RIGHT };

UENUM(BlueprintType, DisplayName = "Game State")
enum class EGameState : uint8 {
	IN_MENU UMETA(DisplayName = "In Menu"),
	READY_SET_GO UMETA(DisplayName = "Ready, Set, Go"),
	PASSING_LEVEL UMETA(DisplayName = "Passing Level"),
	PLAYING_FORWARD UMETA(DisplayName = "Playing Forward"),
	PLAYING_SLOW_TIME_DOWN UMETA(DisplayName = "Playing Slow Time Down"),
	PLAYING_PAUSED_TIME UMETA(DisplayName = "Playing Paused Time"),
	PLAYING_REVERSE UMETA(DisplayName = "Playing Reverse"),
	LEVEL_CLEAR UMETA(DisplayName = "Level Clear"),
	DYING UMETA(DisplayName = "Dying"),
	GAME_OVER UMETA(DisplayName = "Game Over"),
};