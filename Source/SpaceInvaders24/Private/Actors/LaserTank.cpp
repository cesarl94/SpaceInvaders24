

#include "Actors/LaserTank.h"

#include "Core/GS_SpaceInvaders24.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"


ALaserTank::ALaserTank() {
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SceneComponent->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Collider->SetupAttachment(SceneComponent);

	GraphicNodes = CreateDefaultSubobject<USceneComponent>(TEXT("Graphic Nodes"));
	GraphicNodes->SetupAttachment(SceneComponent);
}

void ALaserTank::PossessedBy(AController *NewController) {
	Super::PossessedBy(NewController);

	AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));
	CurrentTexelPosition = GameState->WorldToTexelPos(RootComponent->GetComponentLocation());
	// FVector Texel0X0Y_WorldPos = GameState->TexelToWorldPos(FIntPoint(0, 0));
	// FVector Texel1X0Y_WorldPos = GameState->TexelToWorldPos(FIntPoint(1, 0));
	// FVector WorldDifferenceBetweenTexels = Texel1X0Y_WorldPos - Texel0X0Y_WorldPos;
	// float TexelWorldDifference = WorldDifferenceBetweenTexels.Size();
	// FVector HorizontalMovementWorldDirection = WorldDifferenceBetweenTexels / TexelWorldDifference;
}

void ALaserTank::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (CurrentHorizontalMovement != 0) {
		// We multiply by 60 because the Speed is in texels per frame
		CurrentTexelPosition.X += HorizontalVelocity * DeltaTime * 60.f;

		AGS_SpaceInvaders24 *GameState = Cast<AGS_SpaceInvaders24>(UGameplayStatics::GetGameState(this));

		CurrentTexelPosition.X = FMath::Clamp(CurrentTexelPosition.X, 0, GameState->GetMapSize().X - CharacterSize.X - 1);


		FIntPoint RoundedTexelPosition = FIntPoint(FMath::RoundToInt(CurrentTexelPosition.X), FMath::RoundToInt(CurrentTexelPosition.Y));
		FVector NewWorldPos = GameState->TexelToWorldPos(RoundedTexelPosition);
		RootComponent->SetWorldLocation(NewWorldPos);
	}
}

void ALaserTank::SetHorizontalMovement(float HorizontalMovement) {
	if (HorizontalMovement == CurrentHorizontalMovement) {
		return;
	}

	CurrentHorizontalMovement = HorizontalMovement;
	HorizontalVelocity = HorizontalMovement * Speed;
}
