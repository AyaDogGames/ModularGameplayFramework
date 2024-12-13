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

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYFRAMEWORK_API UDaInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UDaInventoryComponent();

	// Abstract methods for special criteria (to be implemented in subclasses)
	virtual bool IsComplete() const { return false; }
	virtual bool IsEmpty() const { return Items.IsEmpty(); }
	virtual bool IsFull() const { return false; }
	virtual bool IsItemValid(UDaInventoryItemBase* Item) const;
	virtual bool IsItemValid(FGameplayTag Tag) const { return true; }

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItem(UDaInventoryItemBase* Item);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItem(UDaInventoryItemBase* Item);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddItem(UDaInventoryItemBase* Item);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(UDaInventoryItemBase* Item);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	TArray<UDaInventoryItemBase*> GetItems() const { return Items; }
	
protected:
	// Gameplay InventoryTags for static qualities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FGameplayTagContainer InventoryTags;
	
	// Array for replicated inventory items
	UPROPERTY(ReplicatedUsing="OnRep_Items", EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<UDaInventoryItemBase*> Items;

	UFUNCTION()
	void OnRep_Items();
	
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
};

UCLASS(Blueprintable)
class GAMEPLAYFRAMEWORK_API UDaMasterInventory : public UDaInventoryComponent
{
	GENERATED_BODY()

public:
	bool RemoveInventoryItem(UDaInventoryItemBase* Item, bool bRemoveSubItems);
	
};