// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaInventoryComponent.h"
#include "DaEquipmentInventoryComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYFRAMEWORK_API UDaEquipmentInventoryComponent : public UDaInventoryComponent
{
	GENERATED_BODY()

public:
	// Constructor
	UDaEquipmentInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void EquipItem(UDaInventoryItemBase* Item, FName EquipSlotName);

	UFUNCTION(BlueprintCallable)
	void UnequipItem(FName EquipSlotName);

protected:
	// Mapping of equipment slots to items
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Equipment")
	TArray<FName> SlotNames;

	virtual bool AddItem(const UObject* SourceObject, int32 SlotIndex = -1) override;
	virtual bool RemoveItem(int32 SlotIndex = -1) override;
};



