// Copyright Dream Awake Solutions LLC


#include "Inventory/DaInventoryItemBase.h"
#include "Inventory/DaInventoryComponent.h"

#include "AbilitySystemComponent.h"
#include "CoreGameplayTags.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

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

UAbilitySystemComponent* UDaInventoryItemBase::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
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
	}
}

void UDaInventoryItemBase::EndEquipAbility()
{
	// Check for any variations of this tag
	FGameplayTag ItemIDTag = GetSpecificTag(InventoryItemTags, CoreGameplayTags::InventoryItem_EquipAbility);
	if (ItemIDTag.IsValid())
	{
		//TODO: End any activated abilities
	}
}

void UDaInventoryItemBase::OnRep_NestedInventory_Implementation()
{
	//TODO: Notify/Update UI
}

UDaInventoryComponent* UDaInventoryItemBase::GetNestedInventory() const
{
	return NestedInventory;
}

void UDaInventoryItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDaInventoryItemBase, NestedInventory);
}