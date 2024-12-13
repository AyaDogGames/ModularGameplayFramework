// Copyright Dream Awake Solutions LLC


#include "DaInventoryItemBase.h"

#include "AbilitySystemComponent.h"
#include "DaInventoryComponent.h"
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

UDaInventoryComponent* UDaInventoryItemBase::GetNestedInventory() const
{
	return NestedInventory;
}

