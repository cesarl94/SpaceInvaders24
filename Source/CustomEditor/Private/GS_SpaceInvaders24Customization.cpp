#pragma once
#include "GS_SpaceInvaders24Customization.h"

#include "Core/GS_SpaceInvaders24.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "HAL/PlatformApplicationMisc.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "PropertyHandle.h"
#include "UObject/UnrealType.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SWidget.h"
#include "Widgets/Text/STextBlock.h"


void FGS_SpaceInvaders24DetailsCustomization::CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) { DetailBuilder.SortCategories(&SortCustomDetailsCategories); }

// Create the static instance of this detail customization needed for registering it in module startup.
TSharedRef<IDetailCustomization> FGS_SpaceInvaders24DetailsCustomization::MakeInstance() { return MakeShareable(new FGS_SpaceInvaders24DetailsCustomization); }

void FGS_SpaceInvaders24DetailsCustomization::SortCustomDetailsCategories(const TMap<FName, IDetailCategoryBuilder *> &AllCategoryMap) {
	// FName ClassName = "GS_SpaceInvaders24";
	// FName DetailsPanelButton = "UltraZombies";
	// TArray<FName> CategoriesInOrder = {"Turbo Map"};

	// FPropertyEditorModule &PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection(ClassName, DetailsPanelButton, FText::FromName(DetailsPanelButton));

	// for (int32 i = 0, limit = CategoriesInOrder.Num(); i < limit; i++) {
	// 	FName CategoryName = CategoriesInOrder[i];
	// 	if (AllCategoryMap.Contains(CategoryName)) {
	// 		(*AllCategoryMap.Find(CategoryName))->SetSortOrder(i);
	// 	}
	// 	Section->AddCategory(CategoryName);
	// }

	TArray<FString> CategoriesInOrder = {"SpaceInvaders24: Game Data"};

	for (int32 i = 0, limit = CategoriesInOrder.Num(); i < limit; i++) {
		FName CategoryName = FName(CategoriesInOrder[i]);
		if (AllCategoryMap.Contains(CategoryName)) {
			(*AllCategoryMap.Find(CategoryName))->SetSortOrder(i);
		}
	}
}
