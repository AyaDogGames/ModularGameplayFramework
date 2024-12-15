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

	// Overrides to handle slot validation
	virtual bool IsItemValid(UDaInventoryItemBase* Item) const override;

	UFUNCTION(BlueprintCallable, Category="Inventory|Equipment")
	bool EquipItem(UDaInventoryItemBase* Item, FName SlotName);

	UFUNCTION(BlueprintCallable, Category="Inventory|Equipment")
	bool UnequipItem(FName SlotName);

protected:
	// Mapping of equipment slots to items
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Equipment")
	TArray<FName> SlotNames;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipItem(UDaInventoryItemBase* Item, FName SlotName);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UnequipItem(FName SlotName);
};



