// Copyright Dream Awake Solutions LLC

#include "AbilitySystem/Tasks/DaAbilityTask_OnTick.h"

UDaAbilityTask_OnTick::UDaAbilityTask_OnTick(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
}

UDaAbilityTask_OnTick* UDaAbilityTask_OnTick::AbilityTaskOnTick(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UDaAbilityTask_OnTick* MyObj = NewAbilityTask<UDaAbilityTask_OnTick>(OwningAbility, TaskInstanceName);
	return MyObj;
}

void UDaAbilityTask_OnTick::Activate()
{
	Super::Activate();

	// TODO: TICK never runs so we probably need to activate this somehow.
}

void UDaAbilityTask_OnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTick.Broadcast(DeltaTime);
	}
}