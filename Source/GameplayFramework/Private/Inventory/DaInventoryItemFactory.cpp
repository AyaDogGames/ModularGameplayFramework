// Copyright Dream Awake Solutions LLC


#include "Inventory/DaInventoryItemFactory.h"

#include "AbilitySystemComponent.h"
#include "DaItemActor.h"
#include "DaPickupItem.h"
#include "DaPickup_Ability.h"
#include "DaRenderUtilLibrary.h"
#include "GameplayFramework.h"
#include "Inventory/DaStackableInventoryItem.h"


// Add default functionality here for any IDaInventoryItemFactory functions that are not pure virtual.
TSubclassOf<UDaInventoryItemBase> UDaBaseInventoryItemFactory::DetermineInventoryItemClass(
	const UObject* SourceObject) const
{
	if (const ADaItemActor* Pickup = Cast<ADaItemActor>(SourceObject))
	{
		if (Pickup->TypeTags.HasTagExact(CoreGameplayTags::InventoryItem_Stackable))
		{
			return UDaStackableInventoryItem::StaticClass();
		}

		if (Pickup->TypeTags.HasTagExact(CoreGameplayTags::InventoryItem_Equipable))
		{
			// TODO: make an equipableItem class
			return UDaInventoryItemBase::StaticClass();
		}
		
		if (Pickup->TypeTags.HasTagExact(CoreGameplayTags::InventoryItem))
		{
			return UDaInventoryItemBase::StaticClass(); 
		}
	}

	return nullptr;
}

UDaInventoryItemBase* UDaBaseInventoryItemFactory::CreateInventoryItem(const UObject* SourceObject) const
{
	if (const ADaItemActor* Pickup = Cast<ADaItemActor>(SourceObject))
	{
		FDaInventoryItemData Data;
		Data.ItemName = Pickup->Name;
		Data.ItemDescription = Pickup->Description;
		Data.Tags = Pickup->TypeTags;
		Data.AbilitySetToGrant = Pickup->GetAbilitySet();
		Data.ItemClass = DetermineInventoryItemClass(SourceObject);

		if (!FGuid::Parse(Pickup->GetName(), Data.ItemID))
		{
			LOG_WARNING("InventoryItemFactory::CreateInventoryItem: Failed to create FGuid ItemID");
		}

		FVector2d ImageSize = FVector2D(64, 64);
		UTextureRenderTarget2D* ThumbnailRT = UDaRenderUtilLibrary::GenerateThumbnailWithRenderTarget(Pickup->GetMeshComponent(), ImageSize, SourceObject->GetWorld());
		if (ThumbnailRT)
		{
			// Uncomment to Debug write image out to disk
			//
			// if (ThumbnailRT)
			// {
			// 	FTextureRenderTargetResource* RTResource = ThumbnailRT->GameThread_GetRenderTargetResource();
			// 	if (!RTResource)
			// 	{
			// 		UE_LOG(LogTemp, Error, TEXT("Render target resource is invalid."));
			// 	}
			//
			// 	UKismetRenderingLibrary::ExportRenderTarget(SourceObject->GetWorld(), ThumbnailRT, TEXT("C:/Temp/"), TEXT("RenderTargetOutput.png"));
			//

			// check and see if there is a material set to create the thumbnail, and if not fallback to just using the render target directly
			USlateBrushAsset* ThumbnailBrush = nullptr;
			if (UMaterialInterface* BaseMaterial = Pickup->RenderTargetMaterial)
			{
				//UMaterialInterface* BaseMaterial = LoadObject<UMaterialInterface>(SourceObject->GetWorld(), TEXT("Material'/GameplayFramework/Materials/M_RenderTextureIcon.M_RenderTextureIcon'"));
				// Create a dynamic material instance
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, SourceObject->GetWorld());
				if (DynamicMaterial)
				{
					// Assign the render target to the material
					DynamicMaterial->SetTextureParameterValue(FName("DynamicTexture"), ThumbnailRT);
					ThumbnailBrush = UDaRenderUtilLibrary::CreateSlateBrushFromMaterial(DynamicMaterial, ImageSize);
				}
			}

			if (ThumbnailBrush == nullptr)
			{
				// Material method didnt work so fallback to rendertarget
				ThumbnailBrush = UDaRenderUtilLibrary::CreateSlateBrushFromRenderTarget(ThumbnailRT);
			}
			
			if (ThumbnailBrush)
			{
				Data.ThumbnailBrush = ThumbnailBrush;
			}
			else
			{
				LOG_WARNING("InventoryItemFactory::CreateInventoryItem: Failed to create ThumbnailBrush");
			}
		} else
		{
			LOG_WARNING("InventoryItemFactory::CreateInventoryItem: Failed to create ThumbnailRT");
		}
		
		return UDaInventoryItemBase::CreateFromData(Data);
	}

	return nullptr;
}
