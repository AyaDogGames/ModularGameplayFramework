// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaGameplayAbility_BaseTriggeredInputAction.h"
#include "DaGameplayAbility_ClickToMoveAction.generated.h"

class UDaAbilityTask_OnTick;
class USplineComponent;
/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaGameplayAbility_ClickToMoveAction : public UDaGameplayAbility_BaseTriggeredInputAction
{
	GENERATED_BODY()
	
	virtual void OnTriggeredInputAction(const FInputActionValue& Value) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
public:
	UDaGameplayAbility_ClickToMoveAction(const FObjectInitializer& ObjectInitializer);

private:

	UFUNCTION()
	void AutoRun(float Seconds);

	UPROPERTY()
	UDaAbilityTask_OnTick* TickTask = nullptr;
	
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	
};
