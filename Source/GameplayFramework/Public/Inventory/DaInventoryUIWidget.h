// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "UI/DaUserWidgetBase.h"
#include "DaInventoryUIWidget.generated.h"

class UDaInventoryItemBase;
class UDaInventoryComponent;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom))
class GAMEPLAYFRAMEWORK_API UDaInventoryUIWidget : public UDaUserWidgetBase
{
	GENERATED_BODY()

public:
	// Function to bind the widget to an inventory component
	UFUNCTION(BlueprintCallable, Category="UI|Inventory")
	void BindToInventory(UDaInventoryComponent* InventoryComponent);

	// Function to update the UI on inventory change
	UFUNCTION(BlueprintImplementableEvent, Category="UI|Inventory")
	void UpdateInventoryUI(const TArray<UDaInventoryItemBase*>& Items);

protected:
	// Bound inventory component
	UPROPERTY(BlueprintReadOnly, Category="UI|Inventory")
	TObjectPtr<UDaInventoryComponent> BoundInventory;

	// Delegate binding
	UFUNCTION()
	void HandleInventoryChanged(const TArray<UDaInventoryItemBase*>& Items);
};
