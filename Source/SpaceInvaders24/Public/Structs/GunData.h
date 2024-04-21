#pragma once

#include "CoreMinimal.h"
#include "Math/IntPoint.h"
#include "Utils/Enums.h"

#include "GunData.generated.h"


USTRUCT(BlueprintType)
struct SPACEINVADERS24_API FGunData {
	GENERATED_BODY()

	// From where the shot wiil come from, in local coordinates
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FIntPoint Offset;

	// The direction of the shot. Also related with if the owner is the player
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool FromUpToDown;

	// What kind of shots will be fired
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<EShotType> AvailableShotTypes;
};