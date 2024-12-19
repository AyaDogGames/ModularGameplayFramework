// Copyright Dream Awake Solutions LLC


#include "Inventory/DaInventoryItemFactory.h"

#include "AbilitySystemComponent.h"
#include "DaPickupItem.h"
#include "DaPickup_Ability.h"
#include "DaRenderUtilLibrary.h"
#include "Inventory/DaStackableInventoryItem.h"


// Add default functionality here for any IDaInventoryItemFactory functions that are not pure virtual.
TSubclassOf<UDaInventoryItemBase> UDaBaseInventoryItemFactory::DetermineInventoryItemClass(
	const UObject* SourceObject) const
{
	if (const ADaPickupItem* Pickup = Cast<ADaPickupItem>(SourceObject))
	{
		if (Pickup->GetAbilitySystemComponent() &&
			Pickup->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Item.Consumable"))))
		{
			return UDaStackableInventoryItem::StaticClass();
		}
		// else if (Pickup->GetAbilitySystemComponent() &&
		// 		 Pickup->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Item.Equippable"))))
		// {
		// 	return UDaEquippableItem::StaticClass();
		// }
	}

	return nullptr;
}

UDaInventoryItemBase* UDaBaseInventoryItemFactory::CreateInventoryItem(const UObject* SourceObject) const
{
	if (const ADaPickupItem* Pickup = Cast<ADaPickupItem>(SourceObject))
	{
		FDaInventoryItemData Data;
		if (Pickup->GetAbilitySystemComponent())
		{
			Data.Tags = Pickup->GetAbilitySystemComponent()->GetOwnedGameplayTags();
		}

		if (const ADaPickup_Ability* AbilityPickup = Cast<ADaPickup_Ability>(Pickup))
		{
			Data.AbilitySetToGrant = AbilityPickup->GetAbilitySet();
		}

		UTextureRenderTarget2D* ThumbnailRT = UDaRenderUtilLibrary::GenerateThumbnailWithRenderTarget(Pickup->GetMeshComponent(), GetWorld());
		if (ThumbnailRT)
		{
			USlateBrushAsset* ThumbnailBrush = UDaRenderUtilLibrary::CreateSlateBrushFromRenderTarget(ThumbnailRT);
			if (ThumbnailBrush)
				Data.ThumbnailBrush = ThumbnailBrush;
		}
		
		Data.ItemID = FName(Pickup->GetName()); 
		Data.ItemClass = DetermineInventoryItemClass(SourceObject);
		Data.ItemName = FName("Pickup"); // Example item name
		return UDaInventoryItemBase::CreateFromData(Data);
	}

	return nullptr;
}
