// Copyright Dream Awake Solutions LLC


#include "Inventory/DaEquipmentInventoryComponent.h"

#include "Inventory/DaInventoryItemBase.h"


UDaEquipmentInventoryComponent::UDaEquipmentInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

bool UDaEquipmentInventoryComponent::IsItemValid(UDaInventoryItemBase* Item) const
{
	return Super::IsItemValid(Item);
}

bool UDaEquipmentInventoryComponent::EquipItem(UDaInventoryItemBase* Item, FName SlotName)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_EquipItem(Item, SlotName);
		return false;
	}

	int32 SlotIndex = SlotNames.IndexOfByKey(SlotName);
	if (SlotIndex != INDEX_NONE)
	{
		Items[SlotIndex] = Item;
		OnRep_Items();
		Item->ActivateEquipAbility();
		return true;
	}
	return false;
}

bool UDaEquipmentInventoryComponent::UnequipItem(FName SlotName)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_UnequipItem(SlotName);
		return false;
	}

	int32 SlotIndex = SlotNames.IndexOfByKey(SlotName);
	if (SlotIndex != INDEX_NONE)
	{
		if (UDaInventoryItemBase* Item = Items[SlotIndex])
		{
			// stop any equip abilities
			Item->EndEquipAbility();
		}
		
		Items[SlotIndex] = nullptr;
		OnRep_Items();
		return true;
	}
	return false;
}

void UDaEquipmentInventoryComponent::Server_EquipItem_Implementation(UDaInventoryItemBase* Item, FName SlotName)
{
	EquipItem(Item, SlotName);
}

bool UDaEquipmentInventoryComponent::Server_EquipItem_Validate(UDaInventoryItemBase* Item, FName SlotName)
{
	return Item != nullptr;
}

void UDaEquipmentInventoryComponent::Server_UnequipItem_Implementation(FName SlotName)
{
	UnequipItem(SlotName);
}

bool UDaEquipmentInventoryComponent::Server_UnequipItem_Validate(FName SlotName)
{
	return true;
}


