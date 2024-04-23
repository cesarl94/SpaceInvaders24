// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/Enemy.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Utils/Enums.h"

#include "SwarmMind.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwarmMindEvents);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEINVADERS24_API USwarmMind : public UActorComponent {
	GENERATED_BODY()
private:
	// CAUTION: Ultra-private variable
	// Since Unreal doesn't allow 2-dimensional-arrays, we'll use a 1D array indexed :(
	// Use with the functions: "GetEnemyInCoordinate" and "SetEnemyInIndexed2DArray"
	// CAUTION2: Double reference array togheter with the another array of enemies, "TArray<AEnemy *> Enemies".
	UPROPERTY()
	TArray<AEnemy *> _Enemies2D;

	// TODO: comentar esto. Advertir la doble referencia
	UPROPERTY()
	TArray<AEnemy *> Enemies;

	UPROPERTY()
	FIntPoint LastUpdatedEnemyGridID;

	// TODO: Explicar q me guardo esto para cuando recibimos un nuevo estado
	UPROPERTY()
	bool OnBackwards{false};

	UPROPERTY()
	bool MovingToRight{true};

	UPROPERTY()
	bool MovingDown{false};

	// TODO: Explicar esto
	UPROPERTY()
	bool DirectionBlocked{false};

	// TODO: Explicar esto
	UPROPERTY()
	bool GameOverWasDispatched{false};

	/**
	 * What? Why would I store DeltaTimes? Well, if you want a game equal to the original Space Invaders,
	 * you probably want 60 enemy updates per frame, and by not using DeltaTime to multiply the speed, like
	 * in a current game, it is better to accumulate delta times until it "overflows" from the amount necessary
	 * to update one or more enemies and so that the game is not affected by frame drops
	 */
	UPROPERTY()
	float AccumulatedDeltaTime{0};

	// TODO: comentar esto
	UFUNCTION()
	FIntPoint GetCoordinateOfLastAliveEnemyToUpdate() const;

	// TODO: comentar esta función
	UFUNCTION()
	void FixedUpdate();

	UFUNCTION()
	FIntPoint GetNextEnemyGridIDToUpdate(FIntPoint LastGridID) const;

	UFUNCTION()
	AEnemy *GetNextEnemyToUpdate() const;

	// TODO: comentar que hace esto
	UFUNCTION()
	void SetEnemyInIndexed2DArray(int32 X, int32 Y, AEnemy *Enemy);

	UFUNCTION()
	int32 GetHighestTexelYPositionOfAliveEnemy();

	UFUNCTION()
	bool CanEnemiesGoLower();

	// Functions binded to events:

	// Called from Enemy event
	UFUNCTION()
	void OnEnemyTouchBorder(EDirection Direction);

	// Called from Enemy event
	UFUNCTION()
	void OnEnemyDied(AEnemy *EnemyDied, int32 Points);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemies Data")
	TMap<EEnemyType, TSubclassOf<AEnemy>> EnemyClasses;

	// Open preview image of the first level, check the pixel of the first enemy (plus the offset) and put here
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemies Data")
	FIntPoint TexelCoordOfTopLeftEnemyInFirstLevel;

	// Open preview image, move all enemies at the lowest level before touch the LaserTank, and check the pixel of the first enemy (plus the offset)
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemies Data")
	FIntPoint TexelCoordOfTopLeftEnemyInLastLevel;

	// Position Y on screen of the Enemies in the last valid line before GameOver.
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemies Data")
	int32 LastValidTexelCoordY{208};

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemies Data")
	FIntPoint SeparationBetweenEnemies;

	// The count of columns of aliens
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemies Data")
	int32 EnemiesPerRow{11};

	// The type of the enemies by row. From top to bottom
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemies Data")
	TArray<EEnemyType> EnemyTypesByRow;

	// It represents the speed of the entire swarm in texels per frame, but is distributed among all the enemies.
	// This causes the remaining enemies to update faster as there are fewer of them.
	// In the original game, it's 2 texels per frame.
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Enemies Data")
	float SwarmVelocity{2};

public:
	USwarmMind();

	UFUNCTION()
	void ManualInitialize();

	UFUNCTION()
	void ManualReset(int32 Level);

	// Called from GS_SpaceInvaders24
	UFUNCTION()
	void ManualTick(float CrystalDeltaTime, float CrystalTotalSeconds);

	UFUNCTION()
	FIntPoint GetEnemyArraySize() const;

	UFUNCTION()
	const TArray<AEnemy *> &GetEnemies() const;

	UFUNCTION()
	TArray<AEnemy *> GetLiveEnemies() const;

	// TODO: comentar que hace esto
	UFUNCTION()
	AEnemy *GetEnemyInIndexed2DArray(int32 X, int32 Y) const;

	UFUNCTION()
	void OnNewGameState(EGameState NewGameState);

	// Events:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FOnEnemyDie EnemyDiedEvent;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FSwarmMindEvents KilledAllEnemies;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FSwarmMindEvents EnemiesCantGoLower;
};
