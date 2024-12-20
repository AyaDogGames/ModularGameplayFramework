// Copyright Dream Awake Solutions LLC


#include "Inventory/DaInventoryItemBase.h"
#include "Inventory/DaInventoryComponent.h"
#include "CoreGameplayTags.h"
#include "Net/UnrealNetwork.h"

// Create Static Array of Factories
TArray<TScriptInterface<IDaInventoryItemFactory>> UDaInventoryItemBase::Factories;

UDaInventoryItemBase::UDaInventoryItemBase()
{
	// Initialize components only if needed
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		NestedInventory = CreateDefaultSubobject<UDaInventoryComponent>(TEXT("NestedInventory"));
	}
}

void UDaInventoryItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDaInventoryItemBase, NestedInventory);
	DOREPLIFETIME(UDaInventoryItemBase, bIsEmptySlot);
	DOREPLIFETIME(UDaInventoryItemBase, Name);
	DOREPLIFETIME(UDaInventoryItemBase, Description);
	DOREPLIFETIME(UDaInventoryItemBase, ItemID);
	DOREPLIFETIME(UDaInventoryItemBase, InventoryItemTags);
	DOREPLIFETIME(UDaInventoryItemBase, SlotTags);
}

FDaInventoryItemData UDaInventoryItemBase::ToData() const
{
	FDaInventoryItemData Data;
	Data.Tags = InventoryItemTags;
	Data.ItemName = Name;
	Data.ItemDescription = Description;
	Data.ItemID = ItemID;
	Data.ItemClass = GetClass();
	Data.NestedInventorySize = NestedInventory ? NestedInventory->GetMaxSize() : 0;
	Data.AbilitySetToGrant = AbilitySetToGrant;
	Data.ThumbnailBrush = ThumbnailBrush;
	return Data;
}

void UDaInventoryItemBase::ClearData()
{
	bIsEmptySlot = true;
	InventoryItemTags = FGameplayTagContainer();
	NestedInventory = nullptr;
	AbilitySetToGrant = nullptr;
	ThumbnailBrush = nullptr;
	Name = FName();
	Description = FName();
	ItemID = FGuid();
}

FGameplayTag UDaInventoryItemBase::GetType() const
{
	return GetSpecificTag(InventoryItemTags, CoreGameplayTags::InventoryItem_Type);
}


UDaInventoryItemBase* UDaInventoryItemBase::CreateFromData(const FDaInventoryItemData& Data)
{
	UDaInventoryItemBase* NewItem = NewObject<UDaInventoryItemBase>(GetTransientPackage(), Data.ItemClass);
	if (NewItem)
	{
		NewItem->bIsEmptySlot = false;
		NewItem->PopulateWithData(Data);
	}
	return NewItem;
}

void UDaInventoryItemBase::PopulateWithData(const FDaInventoryItemData& Data)
{
	InventoryItemTags = Data.Tags;
	Name = Data.ItemName;
	Description = Data.ItemDescription;
	ItemID = Data.ItemID;
	
	if (Data.ThumbnailBrush)
	{
		ThumbnailBrush = Data.ThumbnailBrush;
	}

	if (Data.AbilitySetToGrant)
	{
		AbilitySetToGrant = Data.AbilitySetToGrant;
	}

	// TODO: CreateNestedInventory( Data.NestedInventorySize );
}

bool UDaInventoryItemBase::CanMergeWith(const UDaInventoryItemBase* OtherItem) const
{
	// subclasses to implement if desired merging behavior
	return false;
}

void UDaInventoryItemBase::MergeWith(UDaInventoryItemBase* OtherItem)
{
	// subclasses to implement if desired merging behavior
}

void UDaInventoryItemBase::ActivateEquipAbility()
{
	// Check for any variations of this tag
	FGameplayTag ItemIDTag = GetSpecificTag(InventoryItemTags, CoreGameplayTags::InventoryItem_EquipAbility);
	if (ItemIDTag.IsValid())
	{
		//TODO: Get ability from AbilitySet and Activate ... AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(ItemIDTag));
		OnInventoryItemUpdated.Broadcast(this);
	}
}

void UDaInventoryItemBase::EndEquipAbility()
{
	// Check for any variations of this tag
	FGameplayTag ItemIDTag = GetSpecificTag(InventoryItemTags, CoreGameplayTags::InventoryItem_EquipAbility);
	if (ItemIDTag.IsValid())
	{
		//TODO: End any activated abilities
		OnInventoryItemUpdated.Broadcast(this);
	}
}

void UDaInventoryItemBase::OnRep_NestedInventory_Implementation()
{
	//Notify/Update UI
	//OnInventoryItemUpdated.Broadcast(this);
}

UDaInventoryComponent* UDaInventoryItemBase::GetNestedInventory() const
{
	return NestedInventory;
}
