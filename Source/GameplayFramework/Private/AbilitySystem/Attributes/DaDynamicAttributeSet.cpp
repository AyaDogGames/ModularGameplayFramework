// Copyright Dream Awake Solutions LLC


#include "AbilitySystem/Attributes/DaDynamicAttributeSet.h"

#include "AbilitySystem/Attributes/DaAttributeSetDataAsset.h"

float UDaDynamicAttributeSet::GetAttributeValue(FName AttributeName) const
{
	if (const FGameplayAttributeData* Attribute = DynamicAttributes.Find(AttributeName))
	{
		return Attribute->GetCurrentValue();
	}
	return 0.0f; 
}

void UDaDynamicAttributeSet::SetAttributeValue(FName AttributeName, float NewValue)
{
	if (FGameplayAttributeData* Attribute = DynamicAttributes.Find(AttributeName))
	{
		Attribute->SetCurrentValue(NewValue);
	}
}

void UDaDynamicAttributeSet::InitializeAttributes(UDaAttributeSetDataAsset* DataAsset)
{
	if (!DataAsset) return;

	for (const FDynamicAttributeDefinition& Definition : DataAsset->Attributes)
	{
		FGameplayAttributeData& Attribute = DynamicAttributes.FindOrAdd(Definition.AttributeName);
		Attribute.SetBaseValue(Definition.DefaultValue);
		Attribute.SetCurrentValue(Definition.DefaultValue);
	}
}
