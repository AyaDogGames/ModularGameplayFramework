// Copyright Dream Awake Solutions LLC


#include "Inventory/DaConsumablesInventoryComponent.h"

#include "Inventory/DaStackableInventoryItem.h"


UDaConsumablesInventoryComponent::UDaConsumablesInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	MaxStackSize = 99;
}

bool UDaConsumablesInventoryComponent::AddItem(UDaInventoryItemBase* Item)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		if (Item)
			Server_AddItem(Item);
	}
	else
	{
		UDaStackableInventoryItem* StackableItem = Cast<UDaStackableInventoryItem>(Item);
		if (!StackableItem)
			return Super::AddItem(Item); // Fallback for non-stackable items

		for (UDaInventoryItemBase* ExistingItem : Items)
		{
			UDaStackableInventoryItem* ExistingStackable = Cast<UDaStackableInventoryItem>(ExistingItem);
			if (ExistingStackable && ExistingStackable->CanStackWith(StackableItem))
			{
				int32 StackableAmount = FMath::Min(StackableItem->Quantity, ExistingStackable->MaxStackSize - ExistingStackable->Quantity);
				ExistingStackable->Quantity += StackableAmount;
				StackableItem->Quantity -= StackableAmount;

				if (StackableItem->Quantity <= 0)
				{
					OnRep_Items();
					return true;
				}
			}
		}

		// Add new stack if no existing stack can take more
		Items.Add(StackableItem);
		OnRep_Items();
		return true;
	}

	return false;
}

bool UDaConsumablesInventoryComponent::RemoveItem(UDaInventoryItemBase* Item)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		if (Item)
			Server_RemoveItem(Item);
	}
	else
	{
		UDaStackableInventoryItem* StackableItem = Cast<UDaStackableInventoryItem>(Item);
		if (StackableItem && StackableItem->Quantity > 0)
		{
			StackableItem->Quantity--;
			if (StackableItem->Quantity == 0)
			{
				Items.RemoveSingle(StackableItem);
				OnRep_Items();
				return true;
			}
		}
	}

	return false;
}

bool UDaConsumablesInventoryComponent::UseItem(UDaInventoryItemBase* Item)
{
	return RemoveItem(Item);
}

void UDaConsumablesInventoryComponent::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

