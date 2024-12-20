// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DaInventoryItemFactory.generated.h"

class UDaInventoryItemBase;

UINTERFACE(MinimalAPI)
class UDaInventoryItemFactory : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEPLAYFRAMEWORK_API IDaInventoryItemFactory
{
	GENERATED_BODY()

public:
	virtual TSubclassOf<UDaInventoryItemBase> DetermineInventoryItemClass(const UObject* SourceObject) const = 0;
	virtual UDaInventoryItemBase* CreateInventoryItem(const UObject* SourceObject) const = 0;
};

UCLASS()
class GAMEPLAYFRAMEWORK_API UDaBaseInventoryItemFactory : public UObject, public IDaInventoryItemFactory
{
	GENERATED_BODY()

public:
	virtual TSubclassOf<UDaInventoryItemBase> DetermineInventoryItemClass(const UObject* SourceObject) const override;
	virtual UDaInventoryItemBase* CreateInventoryItem(const UObject* SourceObject) const override;
};