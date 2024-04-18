#include "CustomEditor.h"

#include "Actors/PWN_Enemy.h"
#include "Core/GS_SpaceInvaders24.h"
#include "GS_SpaceInvaders24Customization.h"
#include "PWN_EnemyCustomization.h"
#include "PropertyEditorDelegates.h"
#include "PropertyEditorModule.h"


IMPLEMENT_GAME_MODULE(FCustomEditorModule, CustomEditor);

void FCustomEditorModule::StartupModule() {
	RegisterClass(AGS_SpaceInvaders24::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FGS_SpaceInvaders24DetailsCustomization::MakeInstance));
	RegisterClass(APWN_Enemy::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FPWN_EnemyDetailsCustomization::MakeInstance));
}

void FCustomEditorModule::ShutdownModule() {
	FPropertyEditorModule &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.UnregisterCustomClassLayout(AGS_SpaceInvaders24::StaticClass()->GetFName());
	PropertyModule.UnregisterCustomClassLayout(APWN_Enemy::StaticClass()->GetFName());
}

void FCustomEditorModule::RegisterClass(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate) {
	FPropertyEditorModule &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(ClassName, DetailLayoutDelegate);
}