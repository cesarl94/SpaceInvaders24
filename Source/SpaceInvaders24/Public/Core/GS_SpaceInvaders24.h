// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
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

	// CAUTION: Ultra-private variable
	// Since Unreal doesn't allow 2-dimensional-arrays, we'll use a 1D array indexed :(
	// Use with the functions: "GetEnemyInCoordinate" and "SetEnemyInIndexed2DArray"
	// CAUTION2: Double reference array togheter with the another array of enemies, "TArray<class AEnemy *> Enemies".
	UPROPERTY()
	TArray<class AEnemy *> _Enemies2D;

	// TODO: comentar esto. Advertir la doble referencia
	UPROPERTY()
	TArray<class AEnemy *> Enemies;

	UPROPERTY()
	class ALaserTank *Player;

	UPROPERTY()
	TArray<class ABunker *> Bunkers;

	UPROPERTY()
	EGameState GameState{EGameState::IN_MENU};

	UFUNCTION()
	void SpawnSwarm();

	UFUNCTION()
	void SpawnPlayer();

	UFUNCTION()
	void SpawnBunkers();

	UFUNCTION()
	void ResetGame();

	// TODO: comentar que hace esto
	UFUNCTION()
	void SetEnemyInIndexed2DArray(int32 X, int32 Y, class AEnemy *Enemy);

	UFUNCTION()
	void SetNewState(EGameState NewGameState);

	// Called from GameTimeManager's event
	UFUNCTION()
	void OnTimeStateFinished();

protected:
	virtual void BeginPlay() override;

	// Components:
	UPROPERTY(VisibleAnywhere)
	class UGameTimeManager *GameTimeManager;

	UPROPERTY(VisibleAnywhere)
	class USwarmMind *SwarmMind;

	// Serialized data:
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Player")
	TSubclassOf<class ALaserTank> PlayerClass;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Player")
	FIntPoint PlayerSpawnPosition;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Enemies")
	TMap<EEnemyType, TSubclassOf<class AEnemy>> EnemyClasses;

	// Open preview image of the first level, check the pixel of the first enemy (plus the offset) and put here
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Enemies")
	FIntPoint TexelCoordOfTopLeftEnemyInFirstLevel;

	// Open preview image, move all enemies at the lowest level before touch the LaserTank, and check the pixel of the first enemy (plus the offset)
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Enemies")
	FIntPoint TexelCoordOfTopLeftEnemyInLastLevel;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Enemies")
	FIntPoint SeparationBetweenEnemies;

	// The count of columns of aliens
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Enemies")
	int32 EnemiesPerRow{11};

	// The type of the enemies by row. From top to bottom
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Enemies")
	TArray<EEnemyType> EnemyTypesByRow;

	// It represents the speed of the entire swarm in texels per frame, but is distributed among all the enemies.
	// This causes the remaining enemies to update faster as there are fewer of them.
	// In the original game, it's 2 texels per frame.
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Enemies")
	float SwarmVelocity{2};

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Bunkers")
	TSubclassOf<AActor> BunkerClass;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Bunkers")
	TArray<FIntPoint> BunkerCoordinates;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Shots")
	TMap<EShotType, TSubclassOf<class AShot>> ShotsClasses;

	UPROPERTY(BlueprintReadOnly, Transient)
	int32 Level{0};

	UPROPERTY(BlueprintReadOnly, Transient)
	int32 Points{0};

	UPROPERTY(BlueprintReadOnly, Transient)
	int32 Lives{3};

public:
	AGS_SpaceInvaders24();


	UFUNCTION()
	EGameState GetGameState() const;

	UFUNCTION()
	const TArray<class AEnemy *> &GetEnemies() const;

	virtual void Tick(float DeltaTime) override;

	// TODO: comentar que hace esto
	UFUNCTION()
	void OnPlayerControllerConnected(class APlayerController *PC);

	// TODO: comentar que hace esto
	UFUNCTION()
	class AEnemy *GetEnemyInIndexed2DArray(int32 X, int32 Y) const;


#pragma region // Wrapped functions from GamePreviewActor
	// TODO: comentar que hace esto
	UFUNCTION(BlueprintCallable)
	FIntPoint GetMapSize() const;

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
