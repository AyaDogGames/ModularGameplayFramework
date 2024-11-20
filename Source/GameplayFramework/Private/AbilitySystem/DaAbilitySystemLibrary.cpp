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
			UDaBaseAttributeSet* AS = ASC->GetAttributeSetForTag(CoreGameplayTags::AttributesVitalCore);
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
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
			UDaBaseAttributeSet* AS = ASC->GetAttributeSetForTag(CoreGameplayTags::AttributesStats);
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return HUD->GetStatMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}