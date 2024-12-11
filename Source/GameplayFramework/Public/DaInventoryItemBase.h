// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "DaInventoryItemBase.generated.h"

class ADaInventoryBase;
class UDaAbilitySystemComponent;
class UDaBaseAttributeSet;



/**
 * 
 */
UCLASS(Blueprintable)
class GAMEPLAYFRAMEWORK_API ADaInventoryItemBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ADaInventoryItemBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FGameplayTagContainer GetTags() const { return InventoryItemTags; }
	
protected:
	// Gameplay InventoryItemTags for static qualities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FGameplayTagContainer InventoryItemTags;
	
	// Attribute Set for dynamic properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TSubclassOf<UDaBaseAttributeSet> AttributeSetClass; // Attribute set defining item-specific attributes

	UPROPERTY(BlueprintReadOnly, Category="Components")
	TObjectPtr<UDaAbilitySystemComponent> AbilitySystemComponent; // Handles dynamic gameplay effects
	
};
