#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, DisplayName = "Player Attribute")
enum class EPlayerAttribute : uint8 {
	Crystals UMETA(DisplayName = "Crystals"),
	MaxCrystals UMETA(DisplayName = "Max Crystals"),
	Points UMETA(DisplayName = "Points"),
	Lives UMETA(DisplayName = "Lives"),
	Level UMETA(DisplayName = "Level"),
};

// Enum: Gameplay Ability System, Ability Input
UENUM(BlueprintType, DisplayName = "Gameplay Ability Input")
enum class EGASAbilityInput : uint8 {
	None UMETA(DisplayName = "None"),
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	Shoot UMETA(DisplayName = "Shoot"),
	SlowDownTime UMETA(DisplayName = "Slow Down Time"),
	StopTime UMETA(DisplayName = "Stop Time"),
	ReverseTime UMETA(DisplayName = "Reverse Time"),
};
