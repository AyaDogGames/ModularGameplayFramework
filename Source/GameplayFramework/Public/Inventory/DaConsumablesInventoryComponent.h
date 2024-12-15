// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaInventoryComponent.h"
#include "DaConsumablesInventoryComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYFRAMEWORK_API UDaConsumablesInventoryComponent : public UDaInventoryComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDaConsumablesInventoryComponent();

	// Stacking behavior
	virtual bool AddItem(UDaInventoryItemBase* Item) override;

	virtual bool RemoveItem(UDaInventoryItemBase* Item) override;
	
	UFUNCTION(BlueprintCallable, Category="Inventory|Consumables")
	bool UseItem(UDaInventoryItemBase* Item);

protected:
	// Optional: Define stack limits
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Consumables")
	int32 MaxStackSize;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
