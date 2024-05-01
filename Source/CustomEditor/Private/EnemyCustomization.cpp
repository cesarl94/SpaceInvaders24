#pragma once
#include "EnemyCustomization.h"

#include "Actors/Enemy.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "EnemyCustomization.h"
#include "HAL/PlatformApplicationMisc.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "PropertyHandle.h"
#include "UObject/UnrealType.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SWidget.h"
#include "Widgets/Text/STextBlock.h"


void FEnemyDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) { DetailBuilder.SortCategories(&SortCustomDetailsCategories); }

// Create the static instance of this detail customization needed for registering it in module startup.
TSharedRef<IDetailCustomization> FEnemyDetailsCustomization::MakeInstance() { return MakeShareable(new FEnemyDetailsCustomization); }

void FEnemyDetailsCustomization::SortCustomDetailsCategories(const TMap<FName, IDetailCategoryBuilder *> &AllCategoryMap) {
	TArray<FString> CategoriesInOrder = {"SpaceInvaders24: Enemy Stats", "SpaceInvaders24: GAS", "SpaceInvaders24: Enemy Gun Data", "SpaceInvaders24: Crystal Drop Data",
										 "SpaceInvaders24: Blast Trace Data"};

	for (int32 i = 0, limit = CategoriesInOrder.Num(); i < limit; i++) {
		FName CategoryName = FName(CategoriesInOrder[i]);
		if (AllCategoryMap.Contains(CategoryName)) {
			(*AllCategoryMap.Find(CategoryName))->SetSortOrder(i);
		}
	}
}
