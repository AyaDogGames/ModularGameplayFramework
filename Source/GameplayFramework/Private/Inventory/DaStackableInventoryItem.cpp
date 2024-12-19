// Copyright Dream Awake Solutions LLC


#include "Inventory/DaStackableInventoryItem.h"

#include "Net/UnrealNetwork.h"

// TODO: FIX to use Data struct instead of expecting a new UDaInventoryItemBase
// InventoryComp->AddItem(NewObject<UDaInventoryItemBase>());
//
// UDaStackableInventoryItem* NewStack = NewObject<UDaStackableInventoryItem>();
// NewStack->Quantity = 1;
// InventoryComp->AddItem(NewStack);
//
// UDaStackableInventoryItem* NewStack2 = NewObject<UDaStackableInventoryItem>();
// NewStack2->Quantity = 10;
// if (NewStack->CanMergeWith(NewStack2))
// {
// 	NewStack->MergeWith(NewStack2);
// }

bool UDaStackableInventoryItem::CanMergeWith(const UDaInventoryItemBase* OtherItem) const
{
	const UDaStackableInventoryItem* StackableItem = Cast<UDaStackableInventoryItem>(OtherItem);
	if (StackableItem && StackableItem->GetClass() == GetClass() && Quantity < MaxStackSize)
	{
		FGameplayTag ThisType = GetType();
		FGameplayTag OtherType = OtherItem->GetType();
		if (ThisType.MatchesTagExact(OtherType))
			return true;
	}
	return false;
}

void UDaStackableInventoryItem::MergeWith(UDaInventoryItemBase* OtherItem)
{
	UDaStackableInventoryItem* StackableItem = Cast<UDaStackableInventoryItem>(OtherItem);
	if (StackableItem)
	{
		// TODO: REPLICATION
		// TODO: DESTROY ITEM WHEN Q=0
		int32 TransferAmount = FMath::Min(StackableItem->Quantity, MaxStackSize - Quantity);
		Quantity += TransferAmount;
		StackableItem->Quantity -= TransferAmount;

		StackQuantityUpdateDelegate.Broadcast(Quantity);
	}
}

// Replication support
void UDaStackableInventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UDaStackableInventoryItem, Quantity);
	DOREPLIFETIME(UDaStackableInventoryItem, MaxStackSize);
}