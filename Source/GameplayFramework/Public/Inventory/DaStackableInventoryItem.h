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
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Inventory|Stacking")
    int32 MaxStackSize = 99;

    // Current quantity in the stack
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Inventory|Stacking")
    int32 Quantity = 1;

	virtual bool CanMergeWith_Implementation(const UDaInventoryItemBase* OtherItem) const override;
	virtual void MergeWith_Implementation(UDaInventoryItemBase* OtherItem) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
