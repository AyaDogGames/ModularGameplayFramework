// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaWidgetController.h"
#include "DaInventoryWidgetController.generated.h"

class UDaInventoryItemBase;

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

protected:

	UFUNCTION()
	void HandleInventoryChanged(const TArray<UDaInventoryItemBase*>& Items);
};
