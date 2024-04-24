#include "Components/SimpleVoxel.h"

USimpleVoxel::USimpleVoxel() { PrimaryComponentTick.bCanEverTick = true; }

void USimpleVoxel::SetEnabled(bool NewEnabled) { Enabled = NewEnabled; }