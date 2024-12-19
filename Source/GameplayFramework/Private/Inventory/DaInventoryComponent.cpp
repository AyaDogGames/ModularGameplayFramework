// Copyright Dream Awake Solutions LLC


#include "Inventory/DaInventoryComponent.h"
#include "Inventory/DaInventoryItemBase.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CoreGameplayTags.h"
#include "GameplayFramework.h"
#include "GameplayTagContainer.h"
#include "Inventory/DaInventoryBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"


UDaInventoryComponent::UDaInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UDaInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeEmptySlots();
}

void UDaInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDaInventoryComponent, Items);
	DOREPLIFETIME(UDaInventoryComponent, MaxSize);
	DOREPLIFETIME(UDaInventoryComponent, FilledSlots);
	DOREPLIFETIME(UDaInventoryComponent, InventoryTags);
	
}

void UDaInventoryComponent::InitializeEmptySlots()
{
	Items.SetNum(MaxSize); // Ensure array is of correct size

	for (int32 i = 0; i < MaxSize; ++i)
	{
		if (!Items[i])
		{
			UDaInventoryItemBase* EmptySlot = NewObject<UDaInventoryItemBase>(this);
			EmptySlot->bIsEmptySlot = true;
			Items[i] = EmptySlot;
		}
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		OnRep_Items(); // Notify clients
	}
}

bool UDaInventoryComponent::IsItemValid(FGameplayTagContainer Tags) const
{
	// get the full ID tag for this item to check for duplicates 
	FGameplayTag ItemIDTag = GetSpecificTag(Tags, CoreGameplayTags::InventoryItem_ID);

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
		if (InventoryTypeTag.IsValid() && Tags.HasTag(InventoryTypeTag))
		{
			if (bAllowedDuplicate || !bDuplicateExists)
			{
				return true;
			}
		}
	}

	LOG_WARNING("Item failed validation due when adding");
	
	return false;
}

bool UDaInventoryComponent::AddItem(const UObject* SourceObject, int32 SlotIndex)
{
	if (!IsValid(SourceObject)) return false;
	
	if (SlotIndex > INDEX_NONE && Items.IsValidIndex(SlotIndex))
	{
		UDaInventoryItemBase* CurrentItem = Items[SlotIndex];
		if (CurrentItem)
		{
			// Check if the current item is not empty
			if (!CurrentItem->bIsEmptySlot)
			{
				// Broadcast the old item's data before replacing it
				FDaInventoryItemData OldData = CurrentItem->ToData();
				
				// Store old data to ensure proper rollback
				if (GetOwnerRole() != ROLE_Authority)
				{
					PredictedItems.Add(SlotIndex, OldData);
				}

				// TODO: Stackableitems merge
				
				CurrentItem->OnInventoryItemRemoved.Broadcast(OldData);
				//NotifyInventoryChanged(SlotIndex);
			}

			// Determine the required subclass
			TSubclassOf<UDaInventoryItemBase> RequiredClass = UDaInventoryBlueprintLibrary::GetInventoryItemClass(SourceObject);
			if (RequiredClass && CurrentItem->GetClass() != RequiredClass)
			{
				// Replace the current item with a new instance of the correct subclass (stackable?)
				CurrentItem = UDaInventoryBlueprintLibrary::CreateInventoryItem(SourceObject);
				Items[SlotIndex] = CurrentItem;
			}

			if (IsItemValid(CurrentItem->GetTags()))
			{
				FDaInventoryItemData NewData;
				NewData.ItemClass = RequiredClass;
				NewData.ItemID = SourceObject->GetFName();
				NewData.Tags = CurrentItem->GetTags();
				NewData.ItemName = NewData.ItemID;
				CurrentItem->PopulateWithData(NewData);
				
				// Sync changes for server authority
				if (GetOwnerRole() != ROLE_Authority)
				{
					// Log item for rollback in case prediction fails
					Server_AddItem(SourceObject, SlotIndex);
				} else
				{
					FilledSlots++;
				}

				OnRep_Items();
				return true;
			}
		}
	}
		
	return false;
}

void UDaInventoryComponent::Server_AddItem_Implementation(const UObject* SourceObject, int32 SlotIndex)
{
	if (AddItem(SourceObject, SlotIndex))
	{
		// Remove from pending if valid
		// Remove the rollback data
		PredictedItems.Remove(SlotIndex);
	}
	else
	{
		// Rollback prediction on client if invalid
		if (GetOwnerRole() == ROLE_SimulatedProxy)
		{
			RollbackPredictedItem(SlotIndex);
		}
	}
}

void UDaInventoryComponent::RollbackPredictedItem(int32 SlotIndex)
{
	if (SlotIndex > INDEX_NONE && Items.IsValidIndex(SlotIndex))
	{
		UDaInventoryItemBase* CurrentItem = Items[SlotIndex];
		FDaInventoryItemData* OriginalData = PredictedItems.Find(SlotIndex);
		if (OriginalData && OriginalData->ItemClass)
		{
			TSubclassOf<UDaInventoryItemBase> RequiredClass = OriginalData->ItemClass;
			if (CurrentItem->GetClass() != RequiredClass)
			{
				// Replace the current item with a new instance of the correct subclass
				CurrentItem = NewObject<UDaInventoryItemBase>(this, RequiredClass);
				Items[SlotIndex] = CurrentItem;
			}

			CurrentItem->PopulateWithData(*OriginalData);

			// Remove the rollback data
			PredictedItems.Remove(SlotIndex);

			// update listeners
			NotifyInventoryChanged(SlotIndex);
		}
	}
}


bool UDaInventoryComponent::RemoveItem(int32 SlotIndex)
{
	if (SlotIndex > INDEX_NONE && Items.IsValidIndex(SlotIndex))
	{
		UDaInventoryItemBase* CurrentItem = Items[SlotIndex];
		if (CurrentItem)
		{
			if (!CurrentItem->bIsEmptySlot)
			{
				// Broadcast the old item's data before replacing it
				FDaInventoryItemData OldData = CurrentItem->ToData();
				CurrentItem->ClearData();
				
				// notify listeners of the item we've removed
				CurrentItem->OnInventoryItemRemoved.Broadcast(OldData);
				
				// Store old data to ensure proper rollback
				if (GetOwnerRole() != ROLE_Authority)
				{
					PredictedItems.Add(SlotIndex, OldData);
					Server_RemoveItem(SlotIndex);
				} else
				{
					FilledSlots--;
				}

				OnRep_Items();
				return true;
			}
		}
	}
	return false;
}

void UDaInventoryComponent::Server_RemoveItem_Implementation(int32 SlotIndex)
{
	if (RemoveItem(SlotIndex))
	{
		// Remove from pending if valid
		PredictedItems.Remove(SlotIndex);
	}
	else
	{
		// Rollback prediction on client if invalid
		if (GetOwnerRole() == ROLE_SimulatedProxy)
		{
			RollbackPredictedItem(SlotIndex);
		}
	}
}

void UDaInventoryComponent::OnRep_Items()
{
	NotifyInventoryChanged(INDEX_NONE);
}

void UDaInventoryComponent::NotifyInventoryChanged(int32 SlotIndex)
{
	OnInventoryChanged.Broadcast(Items, SlotIndex);
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



