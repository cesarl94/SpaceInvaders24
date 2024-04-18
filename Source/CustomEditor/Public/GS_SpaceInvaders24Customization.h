#pragma once
#include "CoreMinimal.h"
#include "DetailCategoryBuilder.h"
#include "IDetailCustomization.h"


// Forward declaration of IPropertyHandle.
class IPropertyHandle;

// Custom Class Details Customization
class FGS_SpaceInvaders24DetailsCustomization : public IDetailCustomization {

public:
	// Function that customizes the Details Panel.

	virtual void CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) override;
	// Returns a static instance of the Details Panel customization.

	static TSharedRef<IDetailCustomization> MakeInstance();

	static void SortCustomDetailsCategories(const TMap<FName, IDetailCategoryBuilder *> &AllCategoryMap);
};