#pragma once

#include "Utils/Enums.h"

#include "CrystalTickeable.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class UCrystalTickeable : public UInterface {
	GENERATED_BODY()
};

class ICrystalTickeable {
	GENERATED_BODY()

public:
	TArray<UObject *> GetListOfActions();

	void NormalTick(float DeltaTime);

	void CrystalTick(float CrystalDeltaTime);
};