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

	UPROPERTY()
	class ALaserTank *Player;

	UPROPERTY()
	TArray<class ABunker *> Bunkers;

	UPROPERTY()
	TArray<class AShot *> Shots;

	UPROPERTY()
	class AEnemy *UFO;

	UPROPERTY()
	EGameState GameState{EGameState::IN_MENU};

	UPROPERTY()
	float LastUFOAppearance{0};

	UFUNCTION()
	void SpawnPlayer();

	UFUNCTION()
	void SpawnBunkers();

	UFUNCTION()
	void SpawnUFO();

	UFUNCTION()
	void ResetGame();

	UFUNCTION()
	void SetNewState(EGameState NewGameState);

	UFUNCTION()
	void UFOAppear();

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

	// Called from UFO's event
	UFUNCTION()
	void OnUFOTouchBorder(EDirection Direction);

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

	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data|UFO")
	float UFOSecondsPerAppearance{5.f};

	// TODO: comentar esto
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpaceInvaders24: Game Data|Shots")
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

	virtual void Tick(float DeltaTime) override;

	// TODO: comentar que hace esto
	UFUNCTION()
	void OnPlayerControllerConnected(class APlayerController *PC);

	// Called from AShot
	UFUNCTION()
	void ReportNewShot(class AShot *NewShot);

	UFUNCTION(BlueprintCallable)
	const TArray<class AShot *> &GetShots() const;


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
#pragma region // Wrapped functions from GameTimeManager

	// Returns the maximum time that we could pass in another time state (only special states count)
	UFUNCTION()
	float GetDurationOfLongestTimeState();

	// TODO: comentar esto
	UFUNCTION(BlueprintCallable)
	float GetNormalGameTotalSeconds();

	// TODO: comentar esto
	UFUNCTION(BlueprintCallable)
	float GetCrystalTotalSeconds();

	// TODO: comentar esto
	UFUNCTION(BlueprintCallable)
	float GetLastDeltaTime();

	// TODO: comentar esto
	UFUNCTION(BlueprintCallable)
	float GetLastCrystalDeltaTime();

#pragma endregion
};
