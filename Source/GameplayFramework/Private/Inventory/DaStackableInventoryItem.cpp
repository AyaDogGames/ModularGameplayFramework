// Copyright Dream Awake Solutions LLC


#include "Inventory/DaStackableInventoryItem.h"

#include "Net/UnrealNetwork.h"

bool UDaStackableInventoryItem::CanStackWith(const UDaStackableInventoryItem* Other) const
{
	// TODO: Check gameplay tags
	return Other && Other->GetClass() == GetClass() && Quantity < MaxStackSize;
}

// Replication support for Quantity
void UDaStackableInventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UDaStackableInventoryItem, Quantity);
}