// Copyright Dream Awake Solutions LLC


#include "Inventory/DaStackableInventoryItem.h"

#include "Net/UnrealNetwork.h"

bool UDaStackableInventoryItem::CanMergeWith_Implementation(const UDaInventoryItemBase* OtherItem) const
{
	const UDaStackableInventoryItem* StackableItem = Cast<UDaStackableInventoryItem>(OtherItem);
	return StackableItem && StackableItem->GetClass() == GetClass() && Quantity < MaxStackSize;
}

void UDaStackableInventoryItem::MergeWith_Implementation(UDaInventoryItemBase* OtherItem)
{
	UDaStackableInventoryItem* StackableItem = Cast<UDaStackableInventoryItem>(OtherItem);
	if (StackableItem)
	{
		int32 TransferAmount = FMath::Min(StackableItem->Quantity, MaxStackSize - Quantity);
		Quantity += TransferAmount;
		StackableItem->Quantity -= TransferAmount;
	}
}

// Replication support
void UDaStackableInventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UDaStackableInventoryItem, Quantity);
	DOREPLIFETIME(UDaStackableInventoryItem, MaxStackSize);
}