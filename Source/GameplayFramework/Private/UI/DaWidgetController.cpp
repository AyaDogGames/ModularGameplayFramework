// Copyright Dream Awake Solutions LLC


#include "UI/DaWidgetController.h"

#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DaAttributeData.h"
#include "AbilitySystem/Attributes/DaBaseAttributeSet.h"

void UDaWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSetTags = WCParams.AttributeSetTags;
}

void UDaWidgetController::BroadcastInitialValues()
{
	for (auto It = AttributeSetTags.CreateConstIterator(); It; ++It)
	{
		if (!It->IsValid()) continue;
		//LOG("Widget Controller Set Tag: %s", It->ToString());

		UDaBaseAttributeSet* AttributeSet = AbilitySystemComponent->GetAttributeSetForTag(*It);
		if (AttributeSet)
		{
			for (auto& Pair: AttributeSet->TagsToAttributes)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value(), AttributeSet);
			}
		}
	}
}

void UDaWidgetController::BindCallbacksToDependencies()
{
	for (auto It = AttributeSetTags.CreateConstIterator(); It; ++It)
	{
		if (!It->IsValid()) continue;
		//LOG("Widget Controller Set Tag: %s", It->ToString());
		
		UDaBaseAttributeSet* AttributeSet = AbilitySystemComponent->GetAttributeSetForTag(*It);
		if (AttributeSet)
		{
			for (auto& Pair: AttributeSet->TagsToAttributes)
			{
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
					[this, Pair, AttributeSet](const FOnAttributeChangeData& Data)
				{
					BroadcastAttributeInfo(Pair.Key, Pair.Value(), AttributeSet);
				});
			}
		}
	}
}

void UDaWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute, UDaBaseAttributeSet* AttributeSet) const
{
	check(AttributeInfo);
	
	FDaAttributeData AttributeData = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	AttributeData.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(AttributeData);
}
