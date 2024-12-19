// Copyright Dream Awake Solutions LLC


#include "Inventory/DaInventoryItemFactory.h"

#include "AbilitySystemComponent.h"
#include "DaPickupItem.h"
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
		Data.Tags = Pickup->GetAbilitySystemComponent()->GetOwnedGameplayTags();
		Data.ItemID = FName("GeneratedFromPickup"); // Example item ID
		Data.ItemClass = DetermineInventoryItemClass(SourceObject);
		Data.ItemName = FName("Pickup"); // Example item name
		return UDaInventoryItemBase::CreateFromData(Data);
	}

	return nullptr;
}
