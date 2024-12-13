// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "DaInventoryItemBase.generated.h"

class UDaAbilitySet;
class UDaAbilitySystemComponent;
class UDaInventoryComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaInventoryItemBase : public UObject, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UDaInventoryItemBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FGameplayTagContainer GetTags() const { return InventoryItemTags; }
	
protected:
	// Gameplay InventoryItemTags for static qualities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FGameplayTagContainer InventoryItemTags;

	UPROPERTY(BlueprintReadOnly, Category="AbilitySystem")
	TObjectPtr<UDaAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category="AbilitySystem")
	TObjectPtr<UDaAbilitySet> AbilitySetToGrant;

public:
	void InitializeAbilitySystemComponent(AActor* OwnerActor);

	// If the item itself can host an inventory
	UFUNCTION(BlueprintCallable, Category="Inventory")
	UDaInventoryComponent* GetNestedInventory() const;

protected:
	UPROPERTY()
	UDaInventoryComponent* NestedInventory;
};
