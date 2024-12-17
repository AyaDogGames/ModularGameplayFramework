// Copyright Dream Awake Solutions LLC


#include "Inventory/DaInventoryComponent.h"
#include "Inventory/DaInventoryItemBase.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CoreGameplayTags.h"
#include "GameplayTagContainer.h"
#include "Inventory/DaStackableInventoryItem.h"
#include "Net/UnrealNetwork.h"


UDaInventoryComponent::UDaInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

bool UDaInventoryComponent::IsItemValid(UDaInventoryItemBase* Item) const
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
		for (UDaInventoryItemBase* ExistingItem : Items)
		{
			UDaStackableInventoryItem* StackableItem = Cast<UDaStackableInventoryItem>(ExistingItem);
			if (ExistingItem->GetTags().HasTagExact(ItemIDTag) && !StackableItem)
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
		// Check if this inventory supports this type of item
		FGameplayTag InventoryTypeTag = GetSpecificTag(InventoryTags, CoreGameplayTags::InventoryItem_Type);
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

void UDaInventoryComponent::RollbackPredictedItem(UDaInventoryItemBase* Item)
{
	if (PendingPredictedItems.Contains(Item))
	{
		PendingPredictedItems.Remove(Item);
		Items.Remove(Item);
		OnRep_Items();  // Sync UI after rollback
	}
}

bool UDaInventoryComponent::AddItem(UDaInventoryItemBase* Item, int32 SlotIndex)
{
	if (Item && GetOwnerRole() == ROLE_Authority)
	{
		if (IsItemValid(Item))
		{
			if (SlotIndex != INDEX_NONE && Items.IsValidIndex(SlotIndex))
			{
				Items[SlotIndex] = Item;
			}
			else
			{
				for (UDaInventoryItemBase* ExistingItem : Items)
				{
					if (ExistingItem && ExistingItem->CanMergeWith(Item))
					{
						ExistingItem->MergeWith(Item);
						OnRep_Items();
						return true;
					}
				}
				Items.Add(Item);
			}
			OnRep_Items();
			return true;
		}
	}
	else
	{
		// Client Side Prediction
		if (IsItemValid(Item))
		{
			if (SlotIndex != INDEX_NONE && Items.IsValidIndex(SlotIndex))
			{
				Items[SlotIndex] = Item;
			}
			else
			{
				for (UDaInventoryItemBase* ExistingItem : Items)
				{
					if (ExistingItem && ExistingItem->CanMergeWith(Item))
					{
						ExistingItem->MergeWith(Item);
						OnRep_Items();
						return true;
					}
				}
				Items.Add(Item);
			}
			OnRep_Items();
			Server_AddItem(Item, SlotIndex);

			// Log item for rollback in case prediction fails
			PendingPredictedItems.Add(Item);
			return true;
		}
	}
	return false;
}

void UDaInventoryComponent::Server_AddItem_Implementation(UDaInventoryItemBase* Item, int32 SlotIndex)
{
	if (AddItem(Item, SlotIndex))
	{
		// Remove from pending if valid
		PendingPredictedItems.Remove(Item);
	}
	else
	{
		// Rollback prediction on client if invalid
		if (GetOwnerRole() == ROLE_SimulatedProxy)
		{
			RollbackPredictedItem(Item);
		}
	}
}

bool UDaInventoryComponent::RemoveItem(UDaInventoryItemBase* Item, int32 SlotIndex)
{
	if (!Item) return false;
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		if (SlotIndex != INDEX_NONE && Items.IsValidIndex(SlotIndex) && Items[SlotIndex] == Item)
		{
			Items[SlotIndex] = nullptr;
		}
		else if (Items.Remove(Item) > 0)
		{
			OnRep_Items(); 
			return true;
		}
	}
	else
	{
		// Client-side prediction
		if (SlotIndex != INDEX_NONE && Items.IsValidIndex(SlotIndex) && Items[SlotIndex] == Item)
		{
			Items[SlotIndex] = nullptr;
		}
		else if (Items.Remove(Item) > 0)
		{
			OnRep_Items();
			Server_RemoveItem(Item, SlotIndex);

			// Log item for rollback in case prediction fails
			PendingPredictedItems.Add(Item);
			return true;
		}
	}
	return false;
}

void UDaInventoryComponent::Server_RemoveItem_Implementation(UDaInventoryItemBase* Item, int32 SlotIndex)
{
	if (RemoveItem(Item, SlotIndex))
	{
		// Remove from pending if valid
		PendingPredictedItems.Remove(Item);
	}
	else
	{
		// Rollback prediction on client if invalid
		if (GetOwnerRole() == ROLE_SimulatedProxy)
		{
			RollbackPredictedItem(Item);
		}
	}
}

void UDaInventoryComponent::OnRep_Items()
{
	NotifyInventoryChanged();
}

void UDaInventoryComponent::NotifyInventoryChanged()
{
	OnInventoryChanged.Broadcast(Items);
}

UDaInventoryComponent* UDaInventoryComponent::GetInventoryFromActor(AActor* Actor)
{
	if (!Actor) return nullptr;
	return Actor->FindComponentByClass<UDaInventoryComponent>();
}

TArray<UDaInventoryItemBase*> UDaInventoryComponent::QueryByTag(FGameplayTagQuery Query) const
{
	TArray<UDaInventoryItemBase*> FilteredItems;
	for (UDaInventoryItemBase* Item : Items)
	{
		if (Query.Matches(Item->GetTags()))
		{
			FilteredItems.Add(Item);
		}
	}
	return FilteredItems;
}

TArray<UDaInventoryItemBase*> UDaInventoryComponent::QueryByAttribute(FGameplayAttribute Attribute, float MinValue,
	float MaxValue) const
{
	TArray<UDaInventoryItemBase*> FilteredItems;
	for (UDaInventoryItemBase* Item : Items)
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

void UDaInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDaInventoryComponent, Items);
	DOREPLIFETIME(UDaInventoryComponent, MaxSize);
}

bool UDaMasterInventory::RemoveInventoryItem(UDaInventoryItemBase* Item, bool bRemoveSubItems)
{
	bool bRemoved = RemoveItem(Item);

	// remove all the items in inventory if this item es tagged as being an inventory 
	if (bRemoved && Item->GetTags().HasTag(CoreGameplayTags::Inventory_Sub) && bRemoveSubItems)
	{
		if (UDaInventoryComponent* SubInventory = Item->GetNestedInventory())
		{
			for (UDaInventoryItemBase* ItemInSub : SubInventory->GetItems())
			{
				RemoveItem(ItemInSub);
			}
		}
	}

	return bRemoved;
}


