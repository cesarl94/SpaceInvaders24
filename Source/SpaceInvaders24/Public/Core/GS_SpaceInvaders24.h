// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/Enemy.h"
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
#include "Structs/EnemyLine.h"
#include "Structs/TimeStateData.h"
#include "Utils/Enums.h"

#include "GS_SpaceInvaders24.generated.h"


UCLASS()
class SPACEINVADERS24_API AGS_SpaceInvaders24 : public AGameStateBase {
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<class ICrystalTickeable *> CrystalTickeables;

	UPROPERTY()
	class AGamePreviewActor *GamePreviewActor;

	UPROPERTY()
	TArray<AEnemy *> Enemies;

protected:
	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UGameTimeManager *GameTimeManager;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data")
	TMap<EEnemyType, TSubclassOf<AEnemy>> EnemyClasses;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data")
	TMap<ETimeState, FTimeStateData> TimeStateData;

	// Open preview image, check the pixel of the first enemy and put here. Each level it will begin closer of the player
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data")
	TArray<FIntPoint> TexelCoordOfTopLeftEnemyByLevel;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data")
	FIntPoint SeparationBetweenEnemies;

	// Disposition of the enemies. From top to bottom
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data")
	TArray<FEnemyLine> EnemyDispositions;

public:
	AGS_SpaceInvaders24();

	UPROPERTY(BlueprintReadOnly, Transient)
	int32 Level{0};

	UPROPERTY(BlueprintReadOnly, Transient)
	int32 Points{0};

#pragma region // Wrapped functions from GamePreviewActor
	UFUNCTION(BlueprintCallable)
	FIntPoint WorldToTexelPos(FVector WorldPos);

	UFUNCTION(BlueprintCallable)
	FVector TexelToWorldPos(FIntPoint TexelPos);

	UFUNCTION(BlueprintCallable)
	FVector GetForward();

	UFUNCTION(BlueprintCallable)
	FVector GetUp();

	UFUNCTION(BlueprintCallable)
	FRotator GetGameObjectOrientation();
#pragma endregion
};
