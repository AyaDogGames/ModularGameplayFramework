// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DaInventoryComponent.generated.h"

struct FGameplayTagQuery;
struct FGameplayAttribute;
class UDaInventoryItemBase;

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, const TArray<UDaInventoryItemBase*>&, Items);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYFRAMEWORK_API UDaInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UDaInventoryComponent();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool IsEmpty() const { return Items.IsEmpty(); }

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool IsItemValid(UDaInventoryItemBase* Item) const;

	//TODO: Implement base class versions of these
	virtual bool IsItemValid(FGameplayTag Tag) const { return true; }
	virtual bool IsComplete() const { return false; }

	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 GetMaxSize() const { return MaxSize; }

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool IsFull() const { return Items.Num() >= MaxSize-1; }
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool AddItem(UDaInventoryItemBase* Item, int32 SlotIndex = -1);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool RemoveItem(UDaInventoryItemBase* Item, int32 SlotIndex = -1);

	UFUNCTION(Server, Reliable)
	void Server_AddItem(UDaInventoryItemBase* Item, int32 SlotIndex = INDEX_NONE);

	UFUNCTION(Server, Reliable)
	void Server_RemoveItem(UDaInventoryItemBase* Item, int32 SlotIndex = INDEX_NONE);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static UDaInventoryComponent* GetInventoryFromActor(AActor* Actor);
	
	// Delegate to notify listeners when inventory changes
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryChanged OnInventoryChanged;
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	TArray<UDaInventoryItemBase*> GetItems() const { return Items; }
	
protected:
	// Gameplay InventoryTags for static qualities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FGameplayTagContainer InventoryTags;
	
	// Array for replicated inventory items
	UPROPERTY(ReplicatedUsing="OnRep_Items", EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<UDaInventoryItemBase*> Items;

	// Maximum  size for this inventory
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 MaxSize = 99;
	
	UFUNCTION()
	void OnRep_Items();

	UPROPERTY()
	TArray<UDaInventoryItemBase*> PendingPredictedItems;

	UFUNCTION()
	void RollbackPredictedItem(UDaInventoryItemBase* Item);
	
	// Decide how items get added based tags it might have 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	TEnumAsByte<EInventoryItemInsertionPolicy> InsertionPolicy = EInventoryItemInsertionPolicy::AddAlways;

	// Checks type and ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	TEnumAsByte<EInventoryItemDuplicationPolicy> DuplicationPolicy = EInventoryItemDuplicationPolicy::AllowDuplicates;
	
	// Query items by tag
	UFUNCTION(BlueprintCallable, Category="Inventory")
	TArray<UDaInventoryItemBase*> QueryByTag(FGameplayTagQuery Query) const;

	// Query items by attribute
	UFUNCTION(BlueprintCallable, Category="Inventory")
	TArray<UDaInventoryItemBase*> QueryByAttribute(FGameplayAttribute Attribute, float MinValue, float MaxValue) const;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void NotifyInventoryChanged();
};

UCLASS(Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaMasterInventory : public UDaInventoryComponent
{
	GENERATED_BODY()

public:
	bool RemoveInventoryItem(UDaInventoryItemBase* Item, bool bRemoveSubItems);
	
};