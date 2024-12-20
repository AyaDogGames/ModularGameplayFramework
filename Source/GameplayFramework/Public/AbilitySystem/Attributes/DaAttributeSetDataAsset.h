// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DaAttributeSetDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FDynamicAttributeDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	FName AttributeName = FName(); // Name of the attribute (e.g., "Health", "Mana")

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float DefaultValue = 0.0f; // Default value for this attribute
};

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEPLAYFRAMEWORK_API UDaAttributeSetDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	TArray<FDynamicAttributeDefinition> Attributes;
	
};
