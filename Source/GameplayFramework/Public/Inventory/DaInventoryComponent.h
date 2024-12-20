// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DaInventoryItemBase.h"
#include "DaInventoryComponent.generated.h"

struct FGameplayTagQuery;
struct FGameplayAttribute;

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

// SlotIndex of -1 means every index should be updated
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged, const TArray<UDaInventoryItemBase*>&, Items, int32, SlotIndex);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYFRAMEWORK_API UDaInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UDaInventoryComponent();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual void InitializeEmptySlots();
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool IsEmpty() const { return Items.IsEmpty(); }

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool IsItemValid(FGameplayTagContainer Tags) const;

	// returns the first SlotIndex where a duplicate of the same ID tag exists
	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual TArray<int32> GetSlotsWithDuplicates(FGameplayTagContainer Tags) const;
	
	// Cannot rely on this for stackable items
	virtual bool IsComplete() const { return FilledSlots >= MaxSize; }
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual int32 GetSize() const { return FilledSlots; }
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 GetMaxSize() const { return MaxSize; }

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool IsFull() const { return Items.Num() >= MaxSize-1; }

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual int32 FindSlot(FGameplayTagContainer Tags) const;
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool AddItem(const UObject* SourceObject, int32 SlotIndex = -1);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	virtual bool RemoveItem(int32 SlotIndex = -1);

	UFUNCTION(Server, Reliable)
	void Server_AddItem(const UObject* SourceObject, int32 SlotIndex = INDEX_NONE);

	UFUNCTION(Server, Reliable)
	void Server_RemoveItem(int32 SlotIndex = INDEX_NONE);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static UDaInventoryComponent* GetInventoryFromActor(AActor* Actor);
	
	// Delegate to notify listeners when inventory changes
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryChanged OnInventoryChanged;
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	TArray<UDaInventoryItemBase*> GetItems() const { return Items; }
	
protected:
	// Gameplay InventoryTags for static qualities
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FGameplayTagContainer InventoryTags;
	
	// Array for replicated inventory items
	UPROPERTY(ReplicatedUsing="OnRep_Items", EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<UDaInventoryItemBase*> Items;

	// Maximum  size for this inventory
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 MaxSize = 99;

	// Maximum  size for this inventory
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 FilledSlots = 0;
	
	UFUNCTION()
	void OnRep_Items();

	UPROPERTY()
	TMap<int32, FDaInventoryItemData> PredictedItems;

	UFUNCTION()
	void RollbackPredictedItem(int32 SlotIndex);
	
	// Decide how items get added based tags it might have 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	TEnumAsByte<EInventoryItemInsertionPolicy> InsertionPolicy = EInventoryItemInsertionPolicy::AddAlways;

	// Checks type tag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	TEnumAsByte<EInventoryItemDuplicationPolicy> DuplicationPolicy = EInventoryItemDuplicationPolicy::AllowDuplicates;
	
	// Query items by tag
	UFUNCTION(BlueprintCallable, Category="Inventory")
	TArray<UDaInventoryItemBase*> QueryByTag(FGameplayTagQuery Query) const;

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void NotifyInventoryChanged(int32 SlotIndex);
};

UCLASS(Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaMasterInventory : public UDaInventoryComponent
{
	GENERATED_BODY()
};