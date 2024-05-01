// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "GUI.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGUIEvent);


UCLASS()
class SPACEINVADERS24_API UGUI : public UUserWidget {
	GENERATED_BODY()

public:
	// Called from GS_SpaceInvaders24
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "GUI ShowNewHighScoreSign")
	void K2_ShowNewHIghScoreSign();

	// Events:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FGUIEvent OnClickPlay;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FGUIEvent OnPassLevelFinished;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FGUIEvent OnReadySetGoFinished;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FGUIEvent OnGameOverFinished;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleAnywhere, Category = "SpaceInvaders24 Events")
	FGUIEvent OnLevelClearFinished;
};
