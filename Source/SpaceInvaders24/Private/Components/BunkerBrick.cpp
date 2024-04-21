#include "Components/BunkerBrick.h"

#include "Components/BoxComponent.h"


UBunkerBrick::UBunkerBrick() { PrimaryComponentTick.bCanEverTick = true; }

void UBunkerBrick::SetEnabled(bool NewEnabled) { Enabled = NewEnabled; }