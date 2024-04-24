

#pragma once

#include "Actors/Crystal.h"
#include "CoreMinimal.h"
#include "Math/IntPoint.h"

#include "CrystalDropData.generated.h"


// TODO: Comentar esto

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
