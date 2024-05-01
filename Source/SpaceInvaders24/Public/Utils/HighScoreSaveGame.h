// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "HighScoreSaveGame.generated.h"


UCLASS()
class SPACEINVADERS24_API UHighScoreSaveGame : public USaveGame {
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 HighScore;
};
