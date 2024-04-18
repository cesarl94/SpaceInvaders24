// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/PWN_Enemy.h"
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Utils/Enums.h"

#include "GS_SpaceInvaders24.generated.h"


/**
 *
 */
UCLASS()
class SPACEINVADERS24_API AGS_SpaceInvaders24 : public AGameStateBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "SpaceInvaders24: Game Data")
	TMap<EEnemyType, TSubclassOf<APWN_Enemy>> EnemyClasses;
};
