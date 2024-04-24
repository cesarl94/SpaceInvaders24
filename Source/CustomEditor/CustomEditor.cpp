#include "CustomEditor.h"

#include "ActorInTexelsCustomization.h"
#include "Actors/ActorInTexels.h"
#include "Actors/Crystal.h"
#include "Actors/Enemy.h"
#include "Actors/GamePreviewActor.h"
#include "Actors/LaserTank.h"
#include "Actors/Shot.h"
#include "Components/GameTimeManager.h"
#include "Components/SwarmMind.h"
#include "Core/GS_SpaceInvaders24.h"
#include "CrystalCustomization.h"
#include "CustomGameplayAbilityCustomization.h"
#include "EnemyCustomization.h"
#include "GAS/CustomGameplayAbility.h"
#include "GS_SpaceInvaders24Customization.h"
#include "GamePreviewActorCustomization.h"
#include "GameTimeManagerCustomization.h"
#include "LaserTankCustomization.h"
#include "PropertyEditorDelegates.h"
#include "PropertyEditorModule.h"
#include "ShotCustomization.h"
#include "SwarmMindCustomization.h"

IMPLEMENT_GAME_MODULE(FCustomEditorModule, CustomEditor);

void FCustomEditorModule::StartupModule() {
	RegisterClass(AGS_SpaceInvaders24::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FGS_SpaceInvaders24DetailsCustomization::MakeInstance));
	RegisterClass(AEnemy::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FEnemyDetailsCustomization::MakeInstance));
	RegisterClass(AGamePreviewActor::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FGamePreviewActorDetailsCustomization::MakeInstance));
	RegisterClass(ALaserTank::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FLaserTankDetailsCustomization::MakeInstance));
	RegisterClass(UCustomGameplayAbility::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FCustomGameplayAbilityDetailsCustomization::MakeInstance));
	RegisterClass(AShot::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FShotDetailsCustomization::MakeInstance));
	RegisterClass(AActorInTexels::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FActorInTexelsDetailsCustomization::MakeInstance));
	RegisterClass(UGameTimeManager::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FGameTimeManagerDetailsCustomization::MakeInstance));
	RegisterClass(USwarmMind::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FSwarmMindDetailsCustomization::MakeInstance));
	RegisterClass(ACrystal::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FCrystalDetailsCustomization::MakeInstance));
}

void FCustomEditorModule::ShutdownModule() {
	FPropertyEditorModule &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.UnregisterCustomClassLayout(AGS_SpaceInvaders24::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(AEnemy::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(AGamePreviewActor::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(ALaserTank::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(UCustomGameplayAbility::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(AShot::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(AActorInTexels::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(UGameTimeManager::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(USwarmMind::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(ACrystal::StaticClass()->GetFName());
}

void FCustomEditorModule::RegisterClass(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate) {
	FPropertyEditorModule &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(ClassName, DetailLayoutDelegate);
}