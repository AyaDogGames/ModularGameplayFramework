// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "DaInteractableInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "DaItemActor.generated.h"

struct FDaInventoryItemData;
class UDaAbilitySystemComponent;
class UDaAbilitySet;
class USphereComponent;

UCLASS(BlueprintType, Blueprintable)
class GAMEPLAYFRAMEWORK_API ADaItemActor : public AActor, public IDaInteractableInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADaItemActor();

	// Interactable interface
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Items")
	FName Name;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Items")
	FName Description;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Items")
	FGameplayTagContainer TypeTags;

	UFUNCTION(BlueprintCallable, Category = "Items")
	UDaAbilitySet* GetAbilitySet() const { return OwnedAbilitySet; }

	UFUNCTION(BlueprintCallable, Category = "Items")
	virtual void AddToInventory(APawn* InstigatorPawn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items|Icon")
	TObjectPtr<UMaterialInterface> RenderTargetMaterial;
	
	static ADaItemActor* CreateFromInventoryItem(const FDaInventoryItemData& InventoryData);
	
	virtual UStaticMeshComponent* GetMeshComponent() const { return MeshComp; }
	
	void GrantSetToActor(UDaAbilitySystemComponent* ReceivingASC);


protected:
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UDaAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category="Items")
	TObjectPtr<UDaAbilitySet> OwnedAbilitySet;
	
	UPROPERTY(BlueprintReadOnly, Category="Items")
	bool bHighlighted = false;

	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
