// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "DaCharacterBase.h"
#include "DaCharacter.generated.h"

class UDaWorldUserWidget;
class UDaAttributeComponent;
class UDaAbilitySystemComponent;

UCLASS()
class GAMEPLAYFRAMEWORK_API ADaCharacter : public ADaCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADaCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability System", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDaAbilitySystemComponent> AbilityComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability System", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDaAttributeComponent> AttributeComp;
	
	void InitAbilitySystem();
	
	UFUNCTION()
	void OnHealthChanged(UDaAttributeComponent* HealthComponent, float OldHealth, float NewHealth, AActor* InstigatorActor);
	
	UFUNCTION()
	void OnDeathStarted(AActor* OwningActor, AActor* InstigatorActor);
	
	UFUNCTION()
	void OnDeathFinished(AActor* OwningActor);

public:	
	// setup for server and client
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_PlayerState() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void InitPlayerHUD() const;
	
};
