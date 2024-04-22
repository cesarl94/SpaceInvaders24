#pragma once

#include "Math/Vector2D.h"
#include "Utils/Enums.h"

#include "TexelWorldObject2.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UTexelWorldObject2 : public UInterface {
	GENERATED_BODY()
};

class ITexelWorldObject2 {
	GENERATED_BODY()

public:
	// FIntVector4

	// TODO: comentar esto
	FVector2D GetTexelPosition();

	// TODO: comentar esto
	void SetTexelPosition(FVector2D NewTexelPosition);

	// TODO: comentar esto
	FVector2D GetTexelVelocity();

	// TODO: comentar esto
	void SetTexelVelocity(FVector2D NewTexelVelocity);
};