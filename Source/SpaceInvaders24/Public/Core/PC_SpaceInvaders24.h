// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "PC_SpaceInvaders24.generated.h"


/**
 *
 */
UCLASS()
class SPACEINVADERS24_API APC_SpaceInvaders24 : public APlayerController {
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
