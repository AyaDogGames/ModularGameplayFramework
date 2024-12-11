// Copyright Dream Awake Solutions LLC


#include "DaInventoryBase.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CoreGameplayTags.h"
#include "DaInventoryItemBase.h"
#include "GameplayTagContainer.h"


ADaInventoryBase::ADaInventoryBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

bool ADaInventoryBase::AddItem(ADaInventoryItemBase* Item)
{
	if (Item && !IsFull())
	{
		// get the full ID tag for this item to check for duplicates 
		FGameplayTag ItemIDTag = GetSpecificTag(Item->GetTags(), CoreGameplayTags::InventoryItem_ID);

		// check for duplicates
		bool bDuplicateExists = false;
		bool bAllowedDuplicate = DuplicationPolicy == EInventoryItemDuplicationPolicy::AllowDuplicates;
		if (ItemIDTag.IsValid())
		{
			for (ADaInventoryItemBase* ExistingItem : Items)
			{
				if (ExistingItem->GetTags().HasTagExact(ItemIDTag))
				{
					bDuplicateExists = true;
					break;
				}
			}
		}

		// Add if item meets insertion and duplication policies
		if (InsertionPolicy == EInventoryItemInsertionPolicy::AddAlways)
		{
			if (bAllowedDuplicate || !bDuplicateExists)
			{
				Items.Add(Item);
				return true;
			}
		}
		else if (InsertionPolicy == EInventoryItemInsertionPolicy::AddOnlyIfTypeTagMatches)
		{
			// Check if this sub inventory type supports this type of item
			FGameplayTag InventoryTypeTag = GetSpecificTag(InventoryItemTags, CoreGameplayTags::InventoryItem_Type);
			if (InventoryTypeTag.IsValid() && Item->GetTags().HasTag(InventoryTypeTag))
			{
				if (bAllowedDuplicate || !bDuplicateExists)
				{
					Items.Add(Item);
					return true;
				}
			}
		}
		
	}
	return false;
}

bool ADaInventoryBase::RemoveItem(ADaInventoryItemBase* Item)
{
	if (Item == nullptr || !Items.Contains(Item))
	{
		return false;
	}
	
	Items.Remove(Item);
	return true;
}

TArray<ADaInventoryItemBase*> ADaInventoryBase::QueryByTag(FGameplayTagQuery Query) const
{
	TArray<ADaInventoryItemBase*> FilteredItems;
	for (ADaInventoryItemBase* Item : Items)
	{
		if (Query.Matches(Item->GetTags()))
		{
			FilteredItems.Add(Item);
		}
	}
	return FilteredItems;
}

TArray<ADaInventoryItemBase*> ADaInventoryBase::QueryByAttribute(FGameplayAttribute Attribute, float MinValue,
	float MaxValue) const
{
	TArray<ADaInventoryItemBase*> FilteredItems;
	for (ADaInventoryItemBase* Item : Items)
	{
		if (Item && Item->GetAbilitySystemComponent())
		{
			float Value = Item->GetAbilitySystemComponent()->GetNumericAttribute(Attribute);
			if (Value >= MinValue && Value <= MaxValue)
			{
				FilteredItems.Add(Item);
			}
		}
	}
	return FilteredItems;
}

bool ADaMasterInventory::RemoveInventoryItem(ADaInventoryItemBase* Item, bool bRemoveSubItems)
{
	bool bRemoved = RemoveItem(Item);

	// remove all the items from the master inventory if the item removed was a sub inventory
	if (bRemoved && Item->GetTags().HasTag(CoreGameplayTags::Inventory_Sub) && bRemoveSubItems)
	{
		if (ADaInventoryBase* SubInventory = Cast<ADaInventoryBase>(Item))
		{
			for (ADaInventoryItemBase* ItemInSub : SubInventory->GetItems())
			{
				RemoveItem(ItemInSub);
			}
		}
	}

	return bRemoved;
}


