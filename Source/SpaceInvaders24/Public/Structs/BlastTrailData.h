

#pragma once

#include "Actors/BlastTrail.h"
#include "CoreMinimal.h"
#include "Math/IntPoint.h"

#include "BlastTrailData.generated.h"

/**
 * Most objects, upon death, trigger a "animation" of 1 frame. Since the objects in our game are ActorsInTexels, what we do is store
 * in this struct the data of which actor to spawn, where, and for how long, as an animation. It does not work for objects with more
 * than 1 frame of animation, such as the LaserTank, but that's an exception.
 */

USTRUCT(BlueprintType)
struct SPACEINVADERS24_API FBlastTrailData {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SpaceInvaders24: Blast Trace Data")
	TSubclassOf<ABlastTrail> BlastTrailActorClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SpaceInvaders24: Blast Trace Data")
	float Duration{0.1};

	// Amount of texels that we'll add to our texel position to spawn the BlastTrailActor
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SpaceInvaders24: Blast Trace Data")
	FIntPoint Offset;
};