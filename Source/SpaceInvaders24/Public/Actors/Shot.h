// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/ActorInTexels.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Math/IntVector.h"
#include "Math/Vector2D.h"
#include "Structs/BlastTrailData.h"
#include "Utils/Enums.h"

#include "Shot.generated.h"


class AShot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShotHit, AShot *, Shot);


UCLASS()
class SPACEINVADERS24_API AShot : public AActorInTexels {
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	// Called from an event of ActorInTexels
	UFUNCTION()
	void TouchLimit(EDirection Direction);

	UFUNCTION()
	void Dissapear();

protected:
	virtual void BeginPlay() override;

	// Serialized data:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: Shot Stats")
	EShotType Type;

	// texels per frame
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: Shot Stats")
	float MovementSpeed{2};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: Shot Stats")
	float AnimationPlayRate{2.5};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: Blast Trace Data")
	FBlastTrailData BlastTrailData;

public:
	AShot();

	UFUNCTION()
	void ManualTick(float DeltaTime);

	UFUNCTION()
	float GetMovementSpeed() const;

	UFUNCTION()
	EShotType GetType() const;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FOnShotHit OnHit;
};
