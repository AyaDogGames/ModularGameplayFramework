// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "DaInventoryItemBase.generated.h"

class UDaAbilitySet;
class UDaAbilitySystemComponent;
class UDaInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemChanged, UDaInventoryItemBase*, item);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
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

	UPROPERTY(ReplicatedUsing="OnRep_NestedInventory", BlueprintReadOnly, Category = "Inventory")
	UDaInventoryComponent* NestedInventory;

	UFUNCTION(BlueprintNativeEvent, Category="Inventory")
	void OnRep_NestedInventory();

	// Delegate to notify listeners when inventory changes
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryItemChanged OnInventoryItemChanged;
	
public:

	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	bool CanMergeWith(const UDaInventoryItemBase* OtherItem) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	void MergeWith(UDaInventoryItemBase* OtherItem);
	
	void InitializeAbilitySystemComponent(AActor* OwnerActor);

	// Tries to activate an ability on this inventory item with the tag InventoryItem.EquipAbility
	UFUNCTION(BlueprintCallable, Category="AbilitySystem")
	void ActivateEquipAbility();

	UFUNCTION(BlueprintCallable, Category="AbilitySystem")
	void EndEquipAbility();
	
	// If the item itself can host an inventory
	UFUNCTION(BlueprintCallable, Category="Inventory")
	UDaInventoryComponent* GetNestedInventory() const;
   
};
