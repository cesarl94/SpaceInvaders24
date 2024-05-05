// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Internationalization/StringTable.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
#include "Utils/Enums.h"

#include "GS_SpaceInvaders24.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewGameState, EGameState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewHighScore, int32, NewHighScore);


UCLASS()
class SPACEINVADERS24_API AGS_SpaceInvaders24 : public AGameStateBase {
	GENERATED_BODY()

private:
	UPROPERTY()
	class AGamePreviewActor *GamePreviewActor;

	UPROPERTY()
	class ALaserTank *Player;

	UPROPERTY()
	TArray<class ABunker *> Bunkers;

	UPROPERTY()
	TArray<class AShot *> Shots;

	UPROPERTY()
	TArray<class ACrystal *> Crystals;

	UPROPERTY()
	class AEnemy *UFO;

	UPROPERTY()
	class UGUI *GUI;

	UPROPERTY()
	EGameState GameState{EGameState::IN_MENU};

	UPROPERTY()
	float LastUFOAppearance{0};

	UPROPERTY()
	FTimerHandle InputTimeHandle;

	UPROPERTY()
	bool MoveEnemiesToUp{false};

	UPROPERTY()
	int32 HighScore{1000};

	UPROPERTY()
	bool AlreadyEmittedNewHighScore{false};

	UFUNCTION()
	void SpawnPlayer();

	UFUNCTION()
	void SpawnBunkers();

	UFUNCTION()
	void SpawnUFO();

	// HardReset means: true for new games, false for new levels
	UFUNCTION()
	void ResetGame(bool HardReset, int32 LevelToLoad = 0);

	UFUNCTION()
	void UFOAppear();

	UFUNCTION()
	void InitNewLevel();

	// Functions binded to events:

	// Called from GameTimeManager's event
	UFUNCTION()
	void OnTimeStateFinished();

	// Called from SwarmMind's event and from UFO's event
	UFUNCTION()
	void OnEnemyDiedEvent(class AEnemy *EnemyDied, int32 PointsGiven);

	// Called from SwarmMind's event
	UFUNCTION()
	void OnEnemiesCantGoLower();

	// Called from SwarmMind's event
	UFUNCTION()
	void OnKilledAllEnemies();

	// Called from Shot's event
	UFUNCTION()
	void OnShotHit(class AShot *Shot);

	// Called from Crystal's event
	UFUNCTION()
	void OnCrystalHit(class ACrystal *Crystal, bool AddCrystalCount);

	// Called from UFO's event
	UFUNCTION()
	void OnUFOTouchBorder(EDirection Direction);

	// Called from LaserTank's event
	UFUNCTION()
	void OnPlayerStartDying();

	// Called from LaserTank's event
	UFUNCTION()
	void OnPlayerFinallyDie();

	// Called from UI
	UFUNCTION()
	void OnGUIClickPlay();

	// Called from UI
	UFUNCTION()
	void OnReadySetGoFinished();

	// Called from UI
	UFUNCTION()
	void OnPassLevelFinished();

	// Called from UI
	UFUNCTION()
	void OnGameOverFinished();

	// Called from UI
	UFUNCTION()
	void OnLevelClearFinished();

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

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Bunkers")
	TSubclassOf<AActor> BunkerClass;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|Bunkers")
	TArray<FIntPoint> BunkerCoordinates;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|UFO")
	TSubclassOf<class AEnemy> UFOClass;

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|UFO")
	FIntPoint UFOSpawnPosition;

	// texels per frame
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|UFO")
	float UFOMovementSpeed{0.5f};

	// How often will a UFO appear, in seconds
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|UFO")
	float UFOSecondsPerAppearance{20.f};

	// References to the shot classes, mapped according to the ShotType enum.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: Game Data|Shots")
	TMap<EShotType, TSubclassOf<class AShot>> ShotsClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: Game Data|Localization")
	UStringTable *ChosenLanguage;

	// Private but blueprint callable functions:
	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Game State", DisplayName = "GS_SpaceInvaders24 GetLocalizatedString")
	FString GetLocalizatedString(FString Key) const;

	UFUNCTION(BlueprintCallable, Category = "SpaceInvaders24: Game State", DisplayName = "GS_SpaceInvaders24 SetGUIReference")
	void SetGUIReference(class UGUI *GUIReference);

public:
	// Public methods:

	AGS_SpaceInvaders24();

	// Called from SwarmMind
	UFUNCTION()
	TSubclassOf<class AShot> GetShotClass(EShotType ShotClassEnum) const;

	UFUNCTION(BlueprintCallable)
	void SetNewState(EGameState NewGameState);

	UFUNCTION(BlueprintCallable)
	EGameState GetGameState() const;

	UFUNCTION(BlueprintCallable)
	int32 GetHighScore() const;

	// Returns true if the player scored the high score
	UFUNCTION(BlueprintCallable)
	bool IsHighScoredPlay() const;

	virtual void Tick(float DeltaTime) override;

	// This function is called from the PlayerController, and here we will link the player's pawn to the PlayerController.
	UFUNCTION()
	void OnPlayerControllerConnected(class APlayerController *PC);

	// Called from AShot
	UFUNCTION()
	void ReportNewShot(class AShot *NewShot);

	// Called from ACrystal
	UFUNCTION()
	void ReportNewCrystal(class ACrystal *NewCrystal);

	UFUNCTION(BlueprintCallable)
	const TArray<class AShot *> &GetShots() const;


#pragma region // Wrapped functions from GamePreviewActor

	// Returns the count of texels of the world. Width in X, Height in Y
	UFUNCTION(BlueprintCallable)
	FIntPoint GetMapSize() const;

	// It takes a 3D world position and returns the position in texels. The relative Z-axis is ignored.
	UFUNCTION(BlueprintCallable)
	FIntPoint WorldToTexelPos(FVector WorldPos) const;

	// It takes a position in texels and returns a 3D world position. The Z-axis is flattened.
	UFUNCTION(BlueprintCallable)
	FVector TexelToWorldPos(FIntPoint TexelPos) const;

	// It returns the forward direction of the game in 3D, meaning the direction in which the enemies are facing.
	UFUNCTION(BlueprintCallable)
	FVector GetForward() const;

	// It returns the up direction of the game in 3D, meaning the direction in which the enemies would look if they were to raise their view by 90°.
	UFUNCTION(BlueprintCallable)
	FVector GetUp() const;

	/**
	 * It returns the orientation that all objects in the game should have if they were part of the preview. This data is
	 * constructed collectively with the forward and up vectors (with forward taking priority).
	 */
	UFUNCTION(BlueprintCallable)
	FRotator GetGameObjectOrientation() const;

#pragma endregion
#pragma region // Wrapped functions from GameTimeManager

	// Returns the maximum time that we could pass in another time state (only special states count)
	UFUNCTION()
	float GetDurationOfLongestTimeState();

	// Obtains the elapsed time since this game started.
	UFUNCTION(BlueprintCallable)
	float GetNormalGameTotalSeconds();

	// Gets the elapsed time since this game started, affected by crystals.
	UFUNCTION(BlueprintCallable)
	float GetCrystalTotalSeconds();

	// Gets the last delta time.
	UFUNCTION(BlueprintCallable)
	float GetLastDeltaTime();

	// Gets the last delta time, affected by crystals.
	UFUNCTION(BlueprintCallable)
	float GetLastCrystalDeltaTime();

#pragma endregion

	// Events:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FOnNewGameState OnNewGameState;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FOnNewHighScore OnNewHighScore;
};
