// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "DaInventoryItemBase.generated.h"

class IDaInventoryItemFactory;
class UDaAbilitySet;
class UDaAbilitySystemComponent;
class UDaInventoryComponent;

USTRUCT(BlueprintType)
struct FDaInventoryItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NestedInventorySize = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UDaInventoryItemBase> ItemClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer Tags = FGameplayTagContainer();

	UPROPERTY(BlueprintReadOnly)
	int32 ItemCount = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemDataRemoved, const FDaInventoryItemData&, itemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemUpdated, UDaInventoryItemBase*, item);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class GAMEPLAYFRAMEWORK_API UDaInventoryItemBase : public UObject, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UDaInventoryItemBase();

	static UDaInventoryItemBase* CreateFromData(const FDaInventoryItemData& Data);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FGameplayTagContainer GetTags() const { return InventoryItemTags; }

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Inventory")
	bool bIsEmptySlot = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Inventory")
	FName Name;
	
	virtual void PopulateWithData(const FDaInventoryItemData& Data);

	virtual FDaInventoryItemData ToData() const;

	virtual void ClearData();

	static TArray<TScriptInterface<IDaInventoryItemFactory>> Factories;

	// Delegate to notify listeners when inventory changes
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryItemDataRemoved OnInventoryItemRemoved;
	
protected:

	// Tags the item has if !bIsEmptySlot
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FGameplayTagContainer InventoryItemTags;

	// Tags that define this inventory slot (and what can be put in it)
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FGameplayTagContainer SlotTags;
	
	UPROPERTY(BlueprintReadOnly, Category="AbilitySystem")
	TObjectPtr<UDaAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category="AbilitySystem")
	TObjectPtr<UDaAbilitySet> AbilitySetToGrant;

	UPROPERTY(ReplicatedUsing="OnRep_NestedInventory", BlueprintReadOnly, Category = "Inventory")
	UDaInventoryComponent* NestedInventory;

	UFUNCTION(BlueprintNativeEvent, Category="Inventory")
	void OnRep_NestedInventory();

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryItemUpdated OnInventoryItemUpdated;
	
public:

	UFUNCTION(BlueprintCallable, Category="AbilitySystem")
	FGameplayTag GetType() const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool CanMergeWith(const UDaInventoryItemBase* OtherItem) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void MergeWith(UDaInventoryItemBase* OtherItem);
	
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