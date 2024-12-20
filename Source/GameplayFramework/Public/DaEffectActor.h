// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaItemActor.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "DaEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum EEffectRemovalPolicy
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class GAMEPLAYFRAMEWORK_API ADaEffectActor : public ADaItemActor
{
	GENERATED_BODY()

protected:
	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	bool bDestroyOnEffectRemoval = false;
	
	// Can be infinite, duration or instant, but application and removal policies will apply differently to each.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	float Level = 1.0f;
	
	// How should this get applied
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TEnumAsByte<EEffectApplicationPolicy> EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// if this is an infinite or duration effect, how will it get removed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TEnumAsByte<EEffectRemovalPolicy> EffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;

	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
	
};
