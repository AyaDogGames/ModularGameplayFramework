// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaInventoryItemBase.h"
#include "DaStackableInventoryItem.generated.h"

// delegate method to update UI When Quantity changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackableItemQuantitySignature, int32, NewQuantity);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class GAMEPLAYFRAMEWORK_API UDaStackableInventoryItem : public UDaInventoryItemBase
{
	GENERATED_BODY()

public:

    // Maximum stack size for this item
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Inventory|Stacking")
    int32 MaxStackSize = 99;

    // Current quantity in the stack
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Inventory|Stacking")
    int32 Quantity = 1;

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FStackableItemQuantitySignature StackQuantityUpdateDelegate;
	
	virtual bool CanMergeWith(const UDaInventoryItemBase* OtherItem) const override;
	virtual void MergeWith(UDaInventoryItemBase* OtherItem) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
