// Copyright Dream Awake Solutions LLC


#include "AbilitySystem/DaAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "CoreGameplayTags.h"
#include "DaPlayerState.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/DaHUD.h"
#include "UI/DaWidgetController.h"

UDaOverlayWidgetController* UDaAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ADaHUD* HUD = Cast<ADaHUD>(PC->GetHUD()))
		{
			ADaPlayerState* PS = PC->GetPlayerState<ADaPlayerState>();
			UDaAbilitySystemComponent* ASC = Cast<UDaAbilitySystemComponent>(PS->GetAbilitySystemComponent());
			const FGameplayTagContainer SetTags = HUD->GetOverlayAttributeSetTags();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, SetTags);
			return HUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UDaStatMenuWidgetController* UDaAbilitySystemLibrary::GetStatMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ADaHUD* HUD = Cast<ADaHUD>(PC->GetHUD()))
		{
			ADaPlayerState* PS = PC->GetPlayerState<ADaPlayerState>();
			UDaAbilitySystemComponent* ASC = Cast<UDaAbilitySystemComponent>(PS->GetAbilitySystemComponent());
			const FGameplayTagContainer SetTags = HUD->GetStatMenuAttributeSetTags();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, SetTags);
			return HUD->GetStatMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UDaInventoryWidgetController* UDaAbilitySystemLibrary::GetInventoryMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ADaHUD* HUD = Cast<ADaHUD>(PC->GetHUD()))
		{
			ADaPlayerState* PS = PC->GetPlayerState<ADaPlayerState>();
			UDaAbilitySystemComponent* ASC = Cast<UDaAbilitySystemComponent>(PS->GetAbilitySystemComponent());
			const FGameplayTagContainer SetTags = HUD->GetInventoryAttributeSetTags();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, SetTags);
			return HUD->GetInventoryWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
