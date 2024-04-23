
#include "Components/GameTimeManager.h"

#include "Core/GS_SpaceInvaders24.h"
#include "Kismet/GameplayStatics.h"
#include "Structs/TimeStateData.h"
#include "Utils/Enums.h"

// void UGameTimeManager::BeginPlay() {
// 	Super::BeginPlay();
// 	AGS_SpaceInvaders24 *GameState = GetOwner<AGS_SpaceInvaders24>();
// }

UGameTimeManager::UGameTimeManager() { PrimaryComponentTick.bCanEverTick = false; }

void UGameTimeManager::ManualReset() {
	AGS_SpaceInvaders24 *GameState = GetOwner<AGS_SpaceInvaders24>();
	GlobalTimeAtLastStateChange = GameState->GetServerWorldTimeSeconds();
}

void UGameTimeManager::ManualTick(float DeltaTime) {
	FTimeStateData *CurrentTimeStateData = TimeStateData.Find(TimeState);
	if (CurrentTimeStateData != nullptr) {
		if (CurrentTimeStateData->Duration > 0) {
			AGS_SpaceInvaders24 *GameState = GetOwner<AGS_SpaceInvaders24>();
			float GlobalTimeSeconds = GameState->GetServerWorldTimeSeconds();
			float TimeSinceLastStateChange = GlobalTimeSeconds - GlobalTimeAtLastStateChange;

			if (TimeSinceLastStateChange > CurrentTimeStateData->Duration) {
				// I emit this event that will be heared by the GS
				OnTimeStateFinished.Broadcast();
				// after emit that event, the GS calls SetNewState so now I have another state and I need to read my StateData again
				CurrentTimeStateData = TimeStateData.Find(TimeState);
			}
		}
		LastDeltaTime = DeltaTime * CurrentTimeStateData->DeltaTimeDilation;
		LastCrystalDeltaTime = DeltaTime * CurrentTimeStateData->DeltaTimeDilationByCrystal;
	} else {
		LastDeltaTime = DeltaTime;
		LastCrystalDeltaTime = DeltaTime;
	}

	NormalGameTotalSeconds += LastDeltaTime;
	CrystalTotalSeconds += LastCrystalDeltaTime;
}

ETimeState UGameTimeManager::GetTimeState() { return TimeState; }

void UGameTimeManager::SetNewState(ETimeState NewTimeState) {
	TimeState = NewTimeState;

	AGS_SpaceInvaders24 *GameState = GetOwner<AGS_SpaceInvaders24>();
	GlobalTimeAtLastStateChange = GameState->GetServerWorldTimeSeconds();
}

float UGameTimeManager::GetDurationOfLongestTimeState() {
	float RetornedValue = -1;

	for (const TPair<ETimeState, FTimeStateData> &Pair : TimeStateData) {
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