#pragma once

#include "CoreMinimal.h"

#include "TimeStateData.generated.h"


USTRUCT(BlueprintType)
struct SPACEINVADERS24_API FTimeStateData {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float DeltaTimeDilationByCrystal{1};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float DeltaTimeDilation{1};

	// How long does a TimeState last in seconds. -1 if that state is unlimited
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Duration{-1};
};