#pragma once

#include "CoreMinimal.h"
#include "Utils/Enums.h"

#include "EnemyLine.generated.h"


USTRUCT(BlueprintType)
struct SPACEINVADERS24_API FEnemyLine {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 EnemyCount{11};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EEnemyType EnemyType;
};