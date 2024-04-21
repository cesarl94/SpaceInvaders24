#include "CustomEditor.h"

#include "Actors/Bunker.h"
#include "Actors/Enemy.h"
#include "Actors/GamePreviewActor.h"
#include "Actors/LaserTank.h"
#include "Actors/MapBound.h"
#include "BunkerCustomization.h"
#include "Core/GS_SpaceInvaders24.h"
#include "EnemyCustomization.h"
#include "GS_SpaceInvaders24Customization.h"
#include "GamePreviewActorCustomization.h"
#include "LaserTankCustomization.h"
#include "MapBoundCustomization.h"
#include "PropertyEditorDelegates.h"
#include "PropertyEditorModule.h"

IMPLEMENT_GAME_MODULE(FCustomEditorModule, CustomEditor);

void FCustomEditorModule::StartupModule() {
	RegisterClass(AGS_SpaceInvaders24::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FGS_SpaceInvaders24DetailsCustomization::MakeInstance));
	RegisterClass(AEnemy::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FEnemyDetailsCustomization::MakeInstance));
	RegisterClass(AMapBound::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FMapBoundDetailsCustomization::MakeInstance));
	RegisterClass(AGamePreviewActor::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FGamePreviewActorDetailsCustomization::MakeInstance));
	RegisterClass(ALaserTank::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FLaserTankDetailsCustomization::MakeInstance));
	RegisterClass(ABunker::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FBunkerDetailsCustomization::MakeInstance));
}

void FCustomEditorModule::ShutdownModule() {
	FPropertyEditorModule &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.UnregisterCustomClassLayout(AGS_SpaceInvaders24::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(AEnemy::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(AMapBound::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(AGamePreviewActor::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(ALaserTank::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(ABunker::StaticClass()->GetFName());
}

void FCustomEditorModule::RegisterClass(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate) {
	FPropertyEditorModule &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(ClassName, DetailLayoutDelegate);
}