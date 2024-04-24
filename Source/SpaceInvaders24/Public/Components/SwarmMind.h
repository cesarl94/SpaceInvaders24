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

	/**
	 * Contains all the enemies, both alive and dead.
	 * CAUTION: Double reference array togheter with the another array of enemies, "TArray<AEnemy *> _Enemies2D".
	 */
	UPROPERTY()
	TArray<AEnemy *> Enemies;

	UPROPERTY()
	FIntPoint LastUpdatedEnemyGridID;

	// TODO: Explain that I'm saving this for when we receive a new state.
	UPROPERTY()
	bool OnBackwards{false};

	UPROPERTY()
	bool MovingToRight{true};

	UPROPERTY()
	bool MovingDown{false};

	/**
	 * Every time the enemies move, they do so as a group, and all of them will perform the same action until one touches an edge.
	 * When one touches the edge, it will unlock this variable and allow them to descend and change direction once the last enemy in the group has been updated.
	 */
	UPROPERTY()
	bool DirectionBlocked{false};

	// Auxiliary variable to prevent triggering the game over event twice.
	UPROPERTY()
	bool GameOverWasDispatched{false};

	/**
	 * Why would I store DeltaTimes? Well, if you want a game equal to the original Space Invaders, you
	 * probably want 60 enemy updates per frame, and by not using DeltaTime to multiply the speed, like
	 * in a current game, it is better to accumulate delta times until it "overflows" from the amount
	 * necessary to update one or more enemies and so that the game is not affected by frame drops
	 */
	UPROPERTY()
	float AccumulatedDeltaTime{0};

	UPROPERTY()
	int32 LiveEnemiesCount;

	/**
	 * Since all enemies move as a group and perform the same action, we need to wait for the last living enemy tobe updated before
	 * making new decisions. With this function, we can find out which is the last living enemy, starting from the top, from right to left.
	 */
	UFUNCTION()
	FIntPoint GetCoordinateOfLastAliveEnemyToUpdate() const;

	/**
	 * It's like a tick and is called 60 times per second, regardless of frame drops. So, we don't need DeltaTime. Similar to Unity's FixedUpdate.
	 * It's called from our ManualTick function, which does receive a DeltaTime and manages calls to this class.
	 *
	 * We use it to update one enemy and then move to the next.
	 */
	UFUNCTION()
	void FixedUpdate();

	UFUNCTION()
	FIntPoint GetNextEnemyGridIDToUpdate(FIntPoint LastGridID) const;

	UFUNCTION()
	AEnemy *GetNextEnemyToUpdate() const;

	// This function is for setting enemies in the _Enemies2D array without directly accessing the variable.
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

	// This function is for get enemies from the _Enemies2D array without directly accessing the variable.
	UFUNCTION()
	AEnemy *GetEnemyInIndexed2DArray(int32 X, int32 Y) const;

	// Called from GS
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
