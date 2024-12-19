// Copyright Dream Awake Solutions LLC


#include "Inventory/DaInventoryItemBase.h"
#include "Inventory/DaInventoryComponent.h"

#include "AbilitySystemComponent.h"
#include "CoreGameplayTags.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

// Create Static Array of Factories
TArray<TScriptInterface<IDaInventoryItemFactory>> UDaInventoryItemBase::Factories;

UDaInventoryItemBase::UDaInventoryItemBase()
{
	// Initialize components only if needed
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		AbilitySystemComponent = CreateDefaultSubobject<UDaAbilitySystemComponent>(TEXT("AbilitySystemComp"));
		AbilitySystemComponent->SetIsReplicated(true);
		AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

		NestedInventory = CreateDefaultSubobject<UDaInventoryComponent>(TEXT("NestedInventory"));
	}
}


void UDaInventoryItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDaInventoryItemBase, NestedInventory);
	DOREPLIFETIME(UDaInventoryItemBase, bIsEmptySlot);
	DOREPLIFETIME(UDaInventoryItemBase, Name);
	DOREPLIFETIME(UDaInventoryItemBase, InventoryItemTags);
	DOREPLIFETIME(UDaInventoryItemBase, SlotTags);
}

FDaInventoryItemData UDaInventoryItemBase::ToData() const
{
	FDaInventoryItemData Data;
	Data.Tags = InventoryItemTags;
	Data.ItemName = Name;
	Data.ItemID = FName(GetNameSafe(this)); // TODO: Generate UUID From Tags
	Data.ItemClass = GetClass();
	Data.NestedInventorySize = NestedInventory ? NestedInventory->GetMaxSize() : 0;
	return Data;
}

void UDaInventoryItemBase::ClearData()
{
	bIsEmptySlot = true;
	InventoryItemTags = FGameplayTagContainer();
	NestedInventory = nullptr;
	Name = FName();
}

UDaInventoryItemBase* UDaInventoryItemBase::CreateFromData(const FDaInventoryItemData& Data)
{
	UDaInventoryItemBase* NewItem = NewObject<UDaInventoryItemBase>(GetTransientPackage(), Data.ItemClass);
	if (NewItem)
	{
		NewItem->PopulateWithData(Data);
	}
	return NewItem;
}

UAbilitySystemComponent* UDaInventoryItemBase::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
}

void UDaInventoryItemBase::PopulateWithData(const FDaInventoryItemData& Data)
{
	// TODO: Fill this out right
	bIsEmptySlot = false;
	InventoryItemTags = Data.Tags;
	Name = Data.ItemName;
	
	// TODO: CreateNestedInventory( Data.NestedInventorySize );
}

bool UDaInventoryItemBase::CanMergeWith_Implementation(const UDaInventoryItemBase* OtherItem) const
{
	// subclasses to implement if desired merging behavior
	return false;
}

void UDaInventoryItemBase::MergeWith_Implementation(UDaInventoryItemBase* OtherItem)
{
	// subclasses to implement if desired merging behavior
}

void UDaInventoryItemBase::InitializeAbilitySystemComponent(AActor* OwnerActor)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(OwnerActor, OwnerActor);
		if (AbilitySetToGrant)
		{
			AbilitySystemComponent->GrantSet(AbilitySetToGrant);
		}
	}
}

void UDaInventoryItemBase::ActivateEquipAbility()
{
	// Check for any variations of this tag
	FGameplayTag ItemIDTag = GetSpecificTag(InventoryItemTags, CoreGameplayTags::InventoryItem_EquipAbility);
	if (ItemIDTag.IsValid())
	{
		//TODO: Get ability from AbilitySet and Activate ... AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(ItemIDTag));
		OnInventoryItemUpdated.Broadcast(this);
	}
}

void UDaInventoryItemBase::EndEquipAbility()
{
	// Check for any variations of this tag
	FGameplayTag ItemIDTag = GetSpecificTag(InventoryItemTags, CoreGameplayTags::InventoryItem_EquipAbility);
	if (ItemIDTag.IsValid())
	{
		//TODO: End any activated abilities
		OnInventoryItemUpdated.Broadcast(this);
	}
}

void UDaInventoryItemBase::OnRep_NestedInventory_Implementation()
{
	//Notify/Update UI
	OnInventoryItemUpdated.Broadcast(this);
}

UDaInventoryComponent* UDaInventoryItemBase::GetNestedInventory() const
{
	return NestedInventory;
}
