#include "Core/GS_SpaceInvaders24.h"

#include "Actors/Enemy.h"
#include "Actors/GamePreviewActor.h"
#include "Core/Components/GameTimeManager.h"
#include "Kismet/GameplayStatics.h"
#include "Math/IntPoint.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Structs/EnemyLine.h"


AGS_SpaceInvaders24::AGS_SpaceInvaders24() {
	PrimaryActorTick.bCanEverTick = true;

	GameTimeManager = CreateDefaultSubobject<UGameTimeManager>(TEXT("Game Time Manager"));
}

void AGS_SpaceInvaders24::BeginPlay() {
	Super::BeginPlay();

	GamePreviewActor = Cast<AGamePreviewActor>(UGameplayStatics::GetActorOfClass(this, AGamePreviewActor::StaticClass()));
	GamePreviewActor->ManualInitialize();

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator GameObjectOrientation = GetGameObjectOrientation();

	FIntPoint PositionDelta = TexelCoordOfTopLeftEnemyByLevel[Level];

	for (int32 i = 0; i < EnemyDispositions.Num(); i++) {
		FEnemyLine EnemyLineData = EnemyDispositions[i];
		for (int32 j = 0; j < EnemyLineData.EnemyCount; j++) {
			TSubclassOf<AEnemy> *EnemySubclass = EnemyClasses.Find(EnemyLineData.EnemyType);
			if (EnemySubclass != nullptr) {
				FVector EnemyWorldPosition = TexelToWorldPos(PositionDelta);
				AEnemy *Enemy = GetWorld()->SpawnActor<AEnemy>(*EnemySubclass, EnemyWorldPosition, GameObjectOrientation, ActorSpawnParams);
				Enemies.Add(Enemy);

				PositionDelta.X += SeparationBetweenEnemies.X;
			}
		}
		PositionDelta.X -= SeparationBetweenEnemies.X * EnemyLineData.EnemyCount;
		PositionDelta.Y += SeparationBetweenEnemies.Y;
	}
}

#pragma region // Wrapped functions from GamePreviewActor
FIntPoint AGS_SpaceInvaders24::WorldToTexelPos(FVector WorldPos) { return GamePreviewActor->WorldToTexelPos(WorldPos); }

FVector AGS_SpaceInvaders24::TexelToWorldPos(FIntPoint TexelPos) { return GamePreviewActor->TexelToWorldPos(TexelPos); }

FVector AGS_SpaceInvaders24::GetForward() { return GamePreviewActor->GetForward(); }

FVector AGS_SpaceInvaders24::GetUp() { return GamePreviewActor->GetUp(); }

FRotator AGS_SpaceInvaders24::GetGameObjectOrientation() { return GamePreviewActor->GetGameObjectOrientation(); }
#pragma endregion