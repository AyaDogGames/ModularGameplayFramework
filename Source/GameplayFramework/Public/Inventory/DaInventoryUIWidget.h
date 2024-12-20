// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "UI/DaUserWidgetBase.h"
#include "DaInventoryUIWidget.generated.h"

class UDaInventoryItemBase;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom))
class GAMEPLAYFRAMEWORK_API UDaInventoryUIWidget : public UDaUserWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (ExposeOnSpawn="true"))
	UDaInventoryItemBase* InventoryItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (ExposeOnSpawn="true"))
	FName SlotName;
	
};
