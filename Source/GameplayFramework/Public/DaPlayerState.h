// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "DaPlayerState.generated.h"

class UDaInventoryComponent;
class ADaPlayerController;
class UDaBaseAttributeSet;
class UGameplayEffect;
class UDaAbilitySystemComponent;
class UDaPawnData;
class UDaSaveGame;
class ADaPlayerState;
class UDaCombatAttributeSet;
class UDaCharacterAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AActor*, InstigatorActor, int32, NewCreditAmount,
											   int32, Delta);
// Event Handler for Personal Record Time
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRecordTimeChanged, ADaPlayerState*, PlayerState, float, NewTime, float, OldRecord);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLevelChanged, ADaPlayerState*, PlayerState, int32, NewLevel, int32, OldLevel);

USTRUCT(BlueprintType)
struct GAMEPLAYFRAMEWORK_API FPlayerCharacterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FPlayerCharacterInfoRow()
	{
		StartupCredits = 10.0f;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId PawnDataInfoId;

	// Credits granted on initial spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StartupCredits;
};

/**
 * 
 */
UCLASS(Config = Game)
class GAMEPLAYFRAMEWORK_API ADaPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADaPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "DA|PlayerState")
	ADaPlayerController* GetDaPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "DA|PlayerState")
	UDaAbilitySystemComponent* GetDaAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Players Master Inventory
	UFUNCTION(BlueprintCallable, Category = "DA|PlayerState")
	UDaInventoryComponent* GetInventoryComponent() const { return InventoryComp; }
	
	void InitializePlayerPawnData();

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UDaSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UDaSaveGame* SaveObject);

	UFUNCTION(BlueprintCallable, Category="Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category="Credits")
	void AdjustCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category="Level")
	void AdjustLevel(int32 NewLevel);

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	
	/* Checks current record and only sets if better time was passed in. */
	UFUNCTION(BlueprintCallable)
	bool UpdatePersonalRecord(float NewTime);
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnCreditsChanged OnCreditsChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnRecordTimeChanged OnRecordTimeChanged;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnLevelChanged OnLevelChanged;

protected:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UDaAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UDaInventoryComponent> InventoryComp;
	
	// Health attribute set used by this actor.
	UPROPERTY(Transient)
	TObjectPtr<const class UDaCharacterAttributeSet> HealthSet;
	// Combat attribute set used by this actor.
	UPROPERTY(Transient)
	TObjectPtr<const class UDaCombatAttributeSet> CombatSet;
	
	UPROPERTY(EditDefaultsOnly, Category="Player Setup")
	TObjectPtr<UDataTable> PlayerPawnDataTable;
	
	// Starting Credits, note, this will be overrided by loading save game data
	UPROPERTY(ReplicatedUsing="OnRep_Credits", EditDefaultsOnly, BlueprintReadOnly, Category="Player Setup")
	int32 Credits = 0;

	// Player's level, this will be overrided by loading save game data
	UPROPERTY(ReplicatedUsing="OnRep_Level", EditDefaultsOnly, BlueprintReadOnly, Category="Player Setup")
	int32 Level = 1;
	
	UPROPERTY()
	TObjectPtr<UDaPawnData> LoadedPawnData;
	
	void OnPlayerPawnDataLoaded(FPrimaryAssetId LoadedId);

	void InitCharacterAttributes(bool bReset = false) const;
	
	UPROPERTY(BlueprintReadOnly)
	float PersonalRecordTime;
	
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};
