
#include "Components/GameTimeManager.h"

#include "Core/GS_SpaceInvaders24.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Structs/TimeStateData.h"
#include "Utils/Enums.h"


UGameTimeManager::UGameTimeManager() { PrimaryComponentTick.bCanEverTick = false; }

void UGameTimeManager::ManualReset() {
	AGS_SpaceInvaders24 *GameState = GetOwner<AGS_SpaceInvaders24>();
	GlobalTimeAtLastStateChange = GameState->GetServerWorldTimeSeconds();
	NormalGameTotalSeconds = 0;
	CrystalTotalSeconds = 0;
}

void UGameTimeManager::ManualTick(float DeltaTime) {
	AGS_SpaceInvaders24 *GameState = GetOwner<AGS_SpaceInvaders24>();

	FTimeStateData *CurrentTimeStateData = TimeStateData.Find(GameState->GetGameState());
	if (CurrentTimeStateData != nullptr) {
		if (CurrentTimeStateData->Duration > 0) {
			float GlobalTimeSeconds = GameState->GetServerWorldTimeSeconds();
			float TimeSinceLastStateChange = GlobalTimeSeconds - GlobalTimeAtLastStateChange;

			if (TimeSinceLastStateChange > CurrentTimeStateData->Duration) {
				// I emit this event that will be heared by the GS
				OnTimeStateFinished.Broadcast();
				// probably after emit that event, the GS has a new state, so I need to read my StateData again
				CurrentTimeStateData = TimeStateData.Find(GameState->GetGameState());
			}
		}
		LastDeltaTime = DeltaTime * CurrentTimeStateData->DeltaTimeDilation;
		LastCrystalDeltaTime = DeltaTime * CurrentTimeStateData->DeltaTimeDilationByCrystal;
	} else {
		LastDeltaTime = 0;
		LastCrystalDeltaTime = 0;
	}

	NormalGameTotalSeconds += LastDeltaTime;
	CrystalTotalSeconds += LastCrystalDeltaTime;
}

void UGameTimeManager::OnNewGameState(EGameState NewGameState) {
	AGS_SpaceInvaders24 *GameState = GetOwner<AGS_SpaceInvaders24>();
	GlobalTimeAtLastStateChange = GameState->GetServerWorldTimeSeconds();
}

float UGameTimeManager::GetDurationOfLongestTimeState() {
	float RetornedValue = -1;

	for (const TPair<EGameState, FTimeStateData> &Pair : TimeStateData) {
		if (RetornedValue < Pair.Value.Duration) {
			RetornedValue = Pair.Value.Duration;
		}
	}

	return RetornedValue;
}

float UGameTimeManager::GetNormalGameTotalSeconds() { return NormalGameTotalSeconds; }

float UGameTimeManager::GetCrystalTotalSeconds() { return CrystalTotalSeconds; }

float UGameTimeManager::GetLastDeltaTime() { return LastDeltaTime; }

float UGameTimeManager::GetLastCrystalDeltaTime() { return LastCrystalDeltaTime; }