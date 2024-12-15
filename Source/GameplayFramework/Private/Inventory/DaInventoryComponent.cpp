// Copyright Dream Awake Solutions LLC


#include "Inventory/DaInventoryComponent.h"
#include "Inventory/DaInventoryItemBase.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CoreGameplayTags.h"
#include "GameplayFramework.h"
#include "GameplayTagContainer.h"
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

bool UDaInventoryComponent::AddItem(UDaInventoryItemBase* Item)
{
	if (GetOwnerRole() == ROLE_Authority)
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

void UDaInventoryComponent::Server_AddItem_Implementation(UDaInventoryItemBase* Item)
{
	AddItem(Item);
}

bool UDaInventoryComponent::Server_AddItem_Validate(UDaInventoryItemBase* Item)
{
	return IsItemValid(Item);
}

bool UDaInventoryComponent::RemoveItem(UDaInventoryItemBase* Item)
{
	if (GetOwnerRole() == ROLE_Authority)
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

void UDaInventoryComponent::Server_RemoveItem_Implementation(UDaInventoryItemBase* Item)
{
	RemoveItem(Item);
}

bool UDaInventoryComponent::Server_RemoveItem_Validate(UDaInventoryItemBase* Item)
{
	return Item != nullptr;
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
}

bool UDaMasterInventory::RemoveInventoryItem(UDaInventoryItemBase* Item, bool bRemoveSubItems)
{
	bool bRemoved = RemoveItem(Item);

	// remove all the items from the master inventory if the item removed was a sub inventory
	if (bRemoved && Item->GetTags().HasTag(CoreGameplayTags::Inventory_Sub) && bRemoveSubItems)
	{
		if (UDaInventoryComponent* SubInventory = Cast<UDaInventoryComponent>(Item))
		{
			for (UDaInventoryItemBase* ItemInSub : SubInventory->GetItems())
			{
				RemoveItem(ItemInSub);
			}
		}
	}

	return bRemoved;
}


