// Copyright Dream Awake Solutions LLC


#include "DaInventoryBase.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CoreGameplayTags.h"
#include "DaInventoryItemBase.h"
#include "GameplayFramework.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"


ADaInventoryBase::ADaInventoryBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

bool ADaInventoryBase::IsItemValid(ADaInventoryItemBase* Item) const
{
	if (!Item || IsFull())
		return false;
	
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
				return true;
			}
		}
	}
	return false;
}

bool ADaInventoryBase::AddItem(ADaInventoryItemBase* Item)
{
	if (HasAuthority())
	{
		if (Item)
		{
			Items.Add(Item);
			OnRep_Items();
			return true;
		}
	} else
	{
		// Client Side Prediction
		if (Item && !IsFull())
		{
			Items.Add(Item);
			OnRep_Items();
			Server_AddItem(Item); // Notify server
			return true;
		}
	}
	return false;
}

void ADaInventoryBase::Server_AddItem_Implementation(ADaInventoryItemBase* Item)
{
	AddItem(Item);
}

bool ADaInventoryBase::Server_AddItem_Validate(ADaInventoryItemBase* Item)
{
	return IsItemValid(Item);
}

bool ADaInventoryBase::RemoveItem(ADaInventoryItemBase* Item)
{
	if (HasAuthority())
	{
		if (Item && Items.Remove(Item) > 0)
		{
			OnRep_Items(); // Notify clients to update UI
			return true;
		}
	}
	else
	{
		// Client-side prediction
		if (Item && Items.Remove(Item) > 0)
		{
			OnRep_Items();
			Server_RemoveItem(Item); // Notify server
			return true;
		}
	}
	return false;
}

void ADaInventoryBase::Server_RemoveItem_Implementation(ADaInventoryItemBase* Item)
{
	RemoveItem(Item);
}

bool ADaInventoryBase::Server_RemoveItem_Validate(ADaInventoryItemBase* Item)
{
	return Item != nullptr;
}

void ADaInventoryBase::OnRep_Items()
{
	// Notify UI or refresh local state upon inventory replication
	for (ADaInventoryItemBase* Item : Items)
	{
		if (Item)
		{
			// Log item replication
			LOG("Replicated Item: %s", *Item->GetName());
			
			// Integrate with UI or update local state as necessary
		}
	}
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

void ADaInventoryBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADaInventoryBase, Items);
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


