// Copyright Dream Awake Solutions LLC


#include "Inventory/DaInventoryUIWidget.h"

#include "Inventory/DaInventoryComponent.h"
#include "Inventory/DaInventoryItemBase.h"
#include "Inventory/DaInventoryWidgetController.h"

void UDaInventoryUIWidget::BindToInventory(UDaInventoryComponent* InventoryComponent)
{
	if (!InventoryComponent)
		return;

	BoundInventory = InventoryComponent;
	BoundInventory->OnInventoryChanged.AddDynamic(this, &UDaInventoryUIWidget::HandleInventoryChanged);

	//UDaInventoryWidgetController* InventoryWidgetController = Cast<UDaInventoryWidgetController>(WidgetController);
}

void UDaInventoryUIWidget::HandleInventoryChanged(const TArray<UDaInventoryItemBase*>& Items)
{
	UpdateInventoryUI(Items);
}
