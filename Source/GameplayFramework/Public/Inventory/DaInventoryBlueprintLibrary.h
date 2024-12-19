// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DaInventoryBlueprintLibrary.generated.h"

class UDaInventoryItemBase;
/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaInventoryBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Inventory")
	static TSubclassOf<UDaInventoryItemBase> GetInventoryItemClass(const UObject* SourceObject);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static UDaInventoryItemBase* CreateInventoryItem(const UObject* SourceObject);
};
