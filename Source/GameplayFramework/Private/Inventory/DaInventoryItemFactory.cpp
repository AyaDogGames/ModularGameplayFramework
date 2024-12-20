// Copyright Dream Awake Solutions LLC


#include "Inventory/DaInventoryItemFactory.h"

#include "AbilitySystemComponent.h"
#include "DaItemActor.h"
#include "DaPickupItem.h"
#include "DaPickup_Ability.h"
#include "DaRenderUtilLibrary.h"
#include "GameplayFramework.h"
#include "Inventory/DaStackableInventoryItem.h"


// Add default functionality here for any IDaInventoryItemFactory functions that are not pure virtual.
TSubclassOf<UDaInventoryItemBase> UDaBaseInventoryItemFactory::DetermineInventoryItemClass(
	const UObject* SourceObject) const
{
	if (const ADaItemActor* Pickup = Cast<ADaItemActor>(SourceObject))
	{
		if (Pickup->TypeTags.HasTagExact(CoreGameplayTags::InventoryItem_Stackable))
		{
			return UDaStackableInventoryItem::StaticClass();
		}

		if (Pickup->TypeTags.HasTagExact(CoreGameplayTags::InventoryItem))
		{
			return UDaInventoryItemBase::StaticClass(); 
		}
	}

	return nullptr;
}

UDaInventoryItemBase* UDaBaseInventoryItemFactory::CreateInventoryItem(const UObject* SourceObject) const
{
	if (const ADaItemActor* Pickup = Cast<ADaItemActor>(SourceObject))
	{
		FDaInventoryItemData Data;
		Data.ItemName = Pickup->Name;
		Data.ItemDescription = Pickup->Description;
		Data.Tags = Pickup->TypeTags;
		Data.AbilitySetToGrant = Pickup->GetAbilitySet();
		Data.ItemClass = DetermineInventoryItemClass(SourceObject);

		if (!FGuid::Parse(Pickup->GetName(), Data.ItemID))
		{
			LOG_WARNING("InventoryItemFactory::CreateInventoryItem: Failed to create FGuid ItemID");
		}

		
		UTextureRenderTarget2D* ThumbnailRT = UDaRenderUtilLibrary::GenerateThumbnailWithRenderTarget(Pickup->GetMeshComponent(), SourceObject->GetWorld());
		if (ThumbnailRT)
		{
			USlateBrushAsset* ThumbnailBrush = UDaRenderUtilLibrary::CreateSlateBrushFromRenderTarget(ThumbnailRT);
			if (ThumbnailBrush)
			{
				Data.ThumbnailBrush = ThumbnailBrush;
			}
			else
			{
				LOG_WARNING("InventoryItemFactory::CreateInventoryItem: Failed to create ThumbnailBrush");
			}
		} else
		{
			LOG_WARNING("InventoryItemFactory::CreateInventoryItem: Failed to create ThumbnailRT");
		}
		
		return UDaInventoryItemBase::CreateFromData(Data);
	}

	return nullptr;
}
