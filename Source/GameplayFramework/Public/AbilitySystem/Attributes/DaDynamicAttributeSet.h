// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "DaBaseAttributeSet.h"
#include "DaDynamicAttributeSet.generated.h"

class UDaAttributeSetDataAsset;
/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaDynamicAttributeSet : public UDaBaseAttributeSet
{
	GENERATED_BODY()

public:

	// Map to hold dynamically defined attributes
	TMap<FName, FGameplayAttributeData> DynamicAttributes;

	// Access attribute by name
	float GetAttributeValue(FName AttributeName) const;
	void SetAttributeValue(FName AttributeName, float NewValue);

	// Initialize attributes from a Data Asset
	void InitializeAttributes(UDaAttributeSetDataAsset* DataAsset);
	
};
