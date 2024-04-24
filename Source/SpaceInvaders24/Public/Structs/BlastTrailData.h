

#pragma once

#include "Actors/ActorInTexels.h"
#include "Actors/BlastTrail.h"
#include "CoreMinimal.h"
#include "Math/IntPoint.h"

#include "BlastTrailData.generated.h"


// TODO: Comentar esto

USTRUCT(BlueprintType)
struct SPACEINVADERS24_API FBlastTrailData {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SpaceInvaders24: Blast Trace Data")
	TSubclassOf<ABlastTrail> BlastTrailActorClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SpaceInvaders24: Blast Trace Data")
	float Duration{0.5};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SpaceInvaders24: Blast Trace Data")
	FIntPoint Offset;
};