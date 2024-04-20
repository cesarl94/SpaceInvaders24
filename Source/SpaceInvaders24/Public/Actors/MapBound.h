// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Color.h"
#include "Utils/Enums.h"

#include "MapBound.generated.h"


UCLASS()
class SPACEINVADERS24_API AMapBound : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMapBound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent *SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent *Mesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent *Collider;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SpaceInvaders24: Game Data")
	TMap<EMapBoundType, FLinearColor> ColorByBoundType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SpaceInvaders24: Game Data")
	EMapBoundType BoundType;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	EMapBoundType GetBoundType() const;
};
