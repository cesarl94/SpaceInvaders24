

#pragma once

#include "Actors/Crystal.h"
#include "CoreMinimal.h"
#include "Math/IntPoint.h"

#include "CrystalDropData.generated.h"

/**
 * The enemies have this struct to fill with data on how to spawn crystals,
 * specifying the class to instantiate, an offset to add to the position of
 * the enemy that dropped it, and the drop probability (in percentage).
 */

USTRUCT(BlueprintType)
struct SPACEINVADERS24_API FCrystalDropData {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Crystal Drop Data")
	TSubclassOf<ACrystal> CrystalDroppedClass;

	// In percentage
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Crystal Drop Data")
	int32 CrystalDropProbability{0};

	// Amount of texels that we'll add to our texel position to spawn the crystal
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Crystal Drop Data")
	FIntPoint CrystalOffset;
};
