#pragma once

#include "CoreMinimal.h"

#include "TimeStateData.generated.h"


USTRUCT(BlueprintType)
struct SPACEINVADERS24_API FTimeStateData {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float TimeDilation{1};

	// How long does a TimeState last. -1 if that state is unlimited
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Duration{-1};
};