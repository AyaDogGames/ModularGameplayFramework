// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DaRenderUtilLibrary.generated.h"

class USlateBrushAsset;
/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API UDaRenderUtilLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Render Utilites")
	static USlateBrushAsset* CreateSlateBrushFromRenderTarget(UTextureRenderTarget2D* RenderTarget);

	UFUNCTION(BlueprintCallable, Category = "Render Utilites")
	static USlateBrushAsset* CreateSlateBrushFromMaterial(UMaterialInstanceDynamic* Material, const FVector2D& ImageSize);
	
	UFUNCTION(BlueprintCallable, Category = "Render Utilites")
	static UTextureRenderTarget2D* GenerateThumbnailWithRenderTarget(UStaticMeshComponent* MeshComp, const FVector2D& ImageSize, UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Render Utilites")
	static void SetupSceneCaptureForThumbnail(USceneCaptureComponent2D* SceneCapture, UStaticMeshComponent* MeshComp);
};
