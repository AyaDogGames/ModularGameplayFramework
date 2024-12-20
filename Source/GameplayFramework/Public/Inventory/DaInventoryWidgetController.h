// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "UI/DaWidgetController.h"
#include "DaInventoryWidgetController.generated.h"

class UDaInventoryComponent;
class UDaInventoryItemBase;

//TODO: DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemClicked, UDaInventoryItemBase*, item, int32, index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemsChanged, const TArray<UDaInventoryItemBase*>&, Items);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemAtIndexChanged, const TArray<UDaInventoryItemBase*>&, Items, int32, SlotIndex);

/**
 * 
 */
UCLASS(Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaInventoryWidgetController : public UDaWidgetController
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "DaInventoryWidgetController")
	void InitializeInventory(AActor* Actor);

	// Delegate to notify listeners when inventory changes
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryItemsChanged OnInventoryChanged;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryItemAtIndexChanged FOnInventoryItemChanged;
	
protected:

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "DaInventoryWidgetController")
	TObjectPtr<UDaInventoryComponent> InventoryComponent;
	
	UFUNCTION()
	void HandleInventoryChanged(const TArray<UDaInventoryItemBase*>& Items, int32 SlotIndex);
};
