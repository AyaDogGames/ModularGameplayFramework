// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DaAbilityTask_OnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickTaskDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaAbilityTask_OnTick : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnTickTaskDelegate OnTick;
	
	UDaAbilityTask_OnTick(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UDaAbilityTask_OnTick* AbilityTaskOnTick(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName);
	
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	
};
