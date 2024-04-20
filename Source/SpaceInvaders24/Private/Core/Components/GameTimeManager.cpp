
#include "Core/Components/GameTimeManager.h"

#include "Core/GS_SpaceInvaders24.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/Enums.h"


UGameTimeManager::UGameTimeManager() { PrimaryComponentTick.bCanEverTick = true; }


void UGameTimeManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	switch (TimeState) {
	case ETimeState::IDLE:
		LastDeltaTime = 0;
		LastCrystalDeltaTime = 0;
		break;
	case ETimeState::FORWARD:
		LastDeltaTime = DeltaTime;
		LastCrystalDeltaTime = DeltaTime;
		break;
	case ETimeState::SLOW:
		LastDeltaTime = DeltaTime;
		LastCrystalDeltaTime = DeltaTime * 0.5f;
		break;
	case ETimeState::PAUSED:
		LastDeltaTime = DeltaTime;
		LastCrystalDeltaTime = 0;
		break;
	case ETimeState::BACKWARD:
		LastDeltaTime = DeltaTime;
		LastCrystalDeltaTime = DeltaTime * -1.f;
		break;
	}

	NormalGameTimeSeconds += LastDeltaTime;
	CrystalTimeSeconds += LastCrystalDeltaTime;
}

void UGameTimeManager::SetNewState(ETimeState NewTimeState) {
	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));
	// GameState->GetServerWorldTimeSeconds()
}