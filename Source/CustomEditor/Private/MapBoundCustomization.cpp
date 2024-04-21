#pragma once
#include "MapBoundCustomization.h"

#include "Actors/MapBound.h"
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


void FMapBoundDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) { DetailBuilder.SortCategories(&SortCustomDetailsCategories); }

// Create the static instance of this detail customization needed for registering it in module startup.
TSharedRef<IDetailCustomization> FMapBoundDetailsCustomization::MakeInstance() { return MakeShareable(new FMapBoundDetailsCustomization); }

void FMapBoundDetailsCustomization::SortCustomDetailsCategories(const TMap<FName, IDetailCategoryBuilder *> &AllCategoryMap) {
	FName ClassName = "MapBound";
	FName DetailsPanelButton = "SpaceInvaders24";
	TArray<FName> CategoriesInOrder = {"SpaceInvaders24: Game Data"};

	FPropertyEditorModule &PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection(ClassName, DetailsPanelButton, FText::FromName(DetailsPanelButton));

	for (int32 i = 0, limit = CategoriesInOrder.Num(); i < limit; i++) {
		FName CategoryName = CategoriesInOrder[i];
		if (AllCategoryMap.Contains(CategoryName)) {
			(*AllCategoryMap.Find(CategoryName))->SetSortOrder(i);
		}
		Section->AddCategory(CategoryName);
	}
}
