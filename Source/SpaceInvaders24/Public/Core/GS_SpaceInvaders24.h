// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
#include "Structs/EnemyRow.h"
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
	TArray<class AEnemy *> Enemies;

	UPROPERTY()
	class APWN_Player *Player;

	UPROPERTY()
	TArray<class ABunker *> Bunkers;

	UFUNCTION()
	void SpawnSwarm();

	UFUNCTION()
	void SpawnPlayer();

	UFUNCTION()
	void SpawnBunkers();

	UFUNCTION()
	void ResetGame();

protected:
	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UGameTimeManager *GameTimeManager;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Player")
	TSubclassOf<APWN_Player> PlayerClass;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Player")
	FIntPoint PlayerSpawnPosition;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Enemies")
	TMap<EEnemyType, TSubclassOf<AEnemy>> EnemyClasses;

	// Open preview image, check the pixel of the first enemy and put here. Each level it will begin closer of the player
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Enemies")
	TArray<FIntPoint> TexelCoordOfTopLeftEnemyByLevel;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Enemies")
	FIntPoint SeparationBetweenEnemies;

	// Disposition of the enemies. From top to bottom
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Enemies")
	TArray<FEnemyRow> EnemyDispositions;

	// It represents the speed of the entire swarm in texels per frame, but distributed among all the enemies.
	// This causes the remaining enemies to update faster as there are fewer of them.
	// In the original game, it's 2 texels per frame.
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Enemies")
	float SwarmVelocity{2};

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Bunkers")
	TSubclassOf<AActor> BunkerClass;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Bunkers")
	TArray<FIntPoint> BunkerCoordinates;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data")
	TMap<ETimeState, FTimeStateData> TimeStateData;

	UPROPERTY(BlueprintReadOnly, Transient)
	int32 Level{0};

	UPROPERTY(BlueprintReadOnly, Transient)
	int32 Points{0};

	UPROPERTY(BlueprintReadOnly, Transient)
	int32 Lives{3};

public:
	AGS_SpaceInvaders24();


	UPROPERTY(BlueprintReadOnly, Transient)
	EGameState GameState{EGameState::IN_MENU};

	UFUNCTION()
	const TArray<AEnemy *> &GetEnemies() const;

	virtual void Tick(float DeltaTime) override;


#pragma region // Wrapped functions from GamePreviewActor
	// TODO: comentar que hace esto
	UFUNCTION(BlueprintCallable)
	FIntPoint WorldToTexelPos(FVector WorldPos) const;

	// TODO: comentar que hace esto
	UFUNCTION(BlueprintCallable)
	FVector TexelToWorldPos(FIntPoint TexelPos) const;

	// TODO: comentar que hace esto
	UFUNCTION(BlueprintCallable)
	FVector GetForward() const;

	// TODO: comentar que hace esto
	UFUNCTION(BlueprintCallable)
	FVector GetUp() const;

	// TODO: comentar que hace esto
	UFUNCTION(BlueprintCallable)
	FRotator GetGameObjectOrientation() const;
#pragma endregion
};
