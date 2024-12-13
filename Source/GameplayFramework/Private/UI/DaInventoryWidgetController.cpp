// Copyright Dream Awake Solutions LLC


#include "UI/DaInventoryWidgetController.h"

#include "DaInventoryComponent.h"
#include "DaInventoryItemBase.h"

void UDaInventoryWidgetController::InitializeInventory(AActor* Actor)
{
	UDaInventoryComponent* Inventory = UDaInventoryComponent::GetInventoryFromActor(Actor);
	if (Inventory)
	{
		// Subscribe to inventory change notifications
		Inventory->OnInventoryChanged.AddDynamic(this, &UDaInventoryWidgetController::HandleInventoryChanged);
	}
}

void UDaInventoryWidgetController::HandleInventoryChanged(const TArray<UDaInventoryItemBase*>& Items)
{
	// Update the UI to reflect the current state of the inventory
	for (const UDaInventoryItemBase* Item : Items)
	{
		// Example: Log item names
		UE_LOG(LogTemp, Log, TEXT("Inventory Item: %s"), *Item->GetName());
	}
	// Trigger your UI update logic here
}