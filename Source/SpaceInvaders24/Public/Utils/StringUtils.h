// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "StringUtils.generated.h"


UCLASS()
class SPACEINVADERS24_API UStringUtils : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StringUtils")
	static FString LeftPad(FString OriginalString, int32 DesiredLength, FString PaddingCharacter) {

		int32 OriginalStringLen = OriginalString.Len();

		FString rv = FString();
		for (int32 i = 0; i < DesiredLength - OriginalStringLen; i++) {
			rv = rv.Append(PaddingCharacter);
		}

		rv = rv.Append(OriginalString);

		return rv;
	}
};
