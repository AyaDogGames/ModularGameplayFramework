// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaInventoryItemBase.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "DaInventoryBase.generated.h"

struct FGameplayTagQuery;
struct FGameplayAttribute;
class ADaInventoryItemBase;

UENUM(BlueprintType)
enum EInventoryItemInsertionPolicy
{
	AddAlways,
	AddOnlyIfTypeTagMatches,
	DoNotAdd
};

UENUM(BlueprintType)
enum EInventoryItemDuplicationPolicy
{
	AllowDuplicates,
	DisallowDuplicates
};

UCLASS(Blueprintable)
class GAMEPLAYFRAMEWORK_API ADaInventoryBase : public ADaInventoryItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADaInventoryBase();

	// Abstract methods for special criteria (to be implemented in subclasses)
	virtual bool IsComplete() const { return false; }
	virtual bool IsEmpty() const { return Items.Num() == 0; }
	virtual bool IsFull() const { return false; }
	virtual bool IsItemValid(ADaInventoryItemBase* Item) const { return true; }
	virtual bool IsItemValid(FGameplayTag Tag) const { return true; }

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool AddItem(ADaInventoryItemBase* Item);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool RemoveItem(ADaInventoryItemBase* Item);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	TArray<ADaInventoryItemBase*> GetItems() const { return Items; }
	
protected:

	// Items in the inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<ADaInventoryItemBase*> Items;

	// Decide how items get added based tags it might have 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	TEnumAsByte<EInventoryItemInsertionPolicy> InsertionPolicy = EInventoryItemInsertionPolicy::AddAlways;

	// Checks type and ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	TEnumAsByte<EInventoryItemDuplicationPolicy> DuplicationPolicy = EInventoryItemDuplicationPolicy::AllowDuplicates;
	
	// Query items by tag
	UFUNCTION(BlueprintCallable, Category="Inventory")
	TArray<ADaInventoryItemBase*> QueryByTag(FGameplayTagQuery Query) const;

	// Query items by attribute
	UFUNCTION(BlueprintCallable, Category="Inventory")
	TArray<ADaInventoryItemBase*> QueryByAttribute(FGameplayAttribute Attribute, float MinValue, float MaxValue) const;
	
};

UCLASS(Blueprintable)
class GAMEPLAYFRAMEWORK_API ADaMasterInventory : public ADaInventoryBase
{
	GENERATED_BODY()

public:
	bool RemoveInventoryItem(ADaInventoryItemBase* Item, bool bRemoveSubItems);
	
};