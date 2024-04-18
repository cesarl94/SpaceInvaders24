#pragma once
#include "PWN_EnemyCustomization.h"

#include "Actors/PWN_Enemy.h"
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


void FPWN_EnemyDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) { DetailBuilder.SortCategories(&SortCustomDetailsCategories); }

// Create the static instance of this detail customization needed for registering it in module startup.
TSharedRef<IDetailCustomization> FPWN_EnemyDetailsCustomization::MakeInstance() { return MakeShareable(new FPWN_EnemyDetailsCustomization); }

void FPWN_EnemyDetailsCustomization::SortCustomDetailsCategories(const TMap<FName, IDetailCategoryBuilder *> &AllCategoryMap) {
	TArray<FString> CategoriesInOrder = {"SpaceInvaders24: Enemy Stats"};

	for (int32 i = 0, limit = CategoriesInOrder.Num(); i < limit; i++) {
		FName CategoryName = FName(CategoriesInOrder[i]);
		if (AllCategoryMap.Contains(CategoryName)) {
			(*AllCategoryMap.Find(CategoryName))->SetSortOrder(i);
		}
	}
}
