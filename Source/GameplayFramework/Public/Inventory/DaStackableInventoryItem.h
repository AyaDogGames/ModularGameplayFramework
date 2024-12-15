// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaInventoryItemBase.h"
#include "DaStackableInventoryItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaStackableInventoryItem : public UDaInventoryItemBase
{
	GENERATED_BODY()

public:
	
    // Maximum stack size for this item
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Stacking")
    int32 MaxStackSize = 99;

    // Current quantity in the stack
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Inventory|Stacking")
    int32 Quantity = 1;

    // Check if the item can stack with another item
    virtual bool CanStackWith(const UDaStackableInventoryItem* Other) const;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
