// Copyright Dream Awake Solutions LLC


#include "DaPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "DaInputComponent.h"
#include "DaPlayerState.h"
#include "Inventory/DaInventoryUIWidget.h"
#include "Inventory/DaInventoryWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/DaHUD.h"
#include "UI/DaUserWidgetBase.h"
#include "UI/DaWidgetController.h"


ADaPlayerController::ADaPlayerController()
{
	bReplicates = true;
	InputType = EGameplayInputType::GameOnly;
}

void ADaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(InputMappingContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ADaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	checkf(InputConfig, TEXT("DaPlayerController requires a reference to a DaInputConfig DataAsset mapping input actions to their respective Input GameplayTag."));
	
	UDaInputComponent* IC = CastChecked<UDaInputComponent>(InputComponent);
	IC->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);

}

void ADaPlayerController::AnyKeyInput(FKey PressedKey)
{
	bIsUsingGamepad = PressedKey.IsGamepadKey();
}

void ADaPlayerController::AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag)
{
	//LogOnScreen(this, FString::Printf(TEXT("Input Pressed: %s"), *InputTag.ToString()), true, FColor::Red, 1.f, 1 );
	if (GetAbilitySystemComponent()) GetAbilitySystemComponent()->AbilityInputTagPressed(Value, InputTag);
}

void ADaPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//LogOnScreen(this, FString::Printf(TEXT("Input Released: %s"), *InputTag.ToString()), true, FColor::Blue, 1.f, 2 );
	if (GetAbilitySystemComponent()) GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
}

void ADaPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//LogOnScreen(this, FString::Printf(TEXT("Input Held: %s"), *InputTag.ToString()), true, FColor::Green, 1.f, 3 );
	if (GetAbilitySystemComponent()) GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
}

UDaAbilitySystemComponent* ADaPlayerController::GetAbilitySystemComponent()
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = Cast<UDaAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()) );
	}

	return AbilitySystemComponent;
}

void ADaPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ADaPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BlueprintBeginPlayingState();
}

void ADaPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);
}

void ADaPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		if (InputType == EGameplayInputType::GameOnly)
		{
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
		else if (InputType == EGameplayInputType::GameAndCursor)
		{
			bShowMouseCursor = true;
			SetInputMode(FInputModeGameAndUI());
		} else
		{
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
		
		//@TODO: Single-player only. Make work for multiplayer.
		// Example issues to resolve: triggering abilities while the game is paused, or releasing your sprint button after pausing the game 
		if (GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(this, false);
		}
		
		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());

		//@TODO: Single-player only. Make work for multiplayer
		if (GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(this, true);
		}
	}
	
}
