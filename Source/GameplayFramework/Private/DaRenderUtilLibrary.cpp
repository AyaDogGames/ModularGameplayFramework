// Copyright Dream Awake Solutions LLC


#include "DaRenderUtilLibrary.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/StaticMesh.h"
#include "Slate/SlateBrushAsset.h"

void UDaRenderUtilLibrary::SetupSceneCaptureForThumbnail(USceneCaptureComponent2D* SceneCapture, UStaticMeshComponent* MeshComp)
{
    if (!SceneCapture || !MeshComp)
    {
        return;
    }

    // Set the scene capture to focus on the mesh
    SceneCapture->ShowOnlyComponent(MeshComp);
    SceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
    SceneCapture->bCaptureEveryFrame = false;
    SceneCapture->bCaptureOnMovement = false;

    // Adjust the transform to frame the mesh
    FVector MeshBounds = MeshComp->Bounds.BoxExtent;
    FVector MeshOrigin = MeshComp->Bounds.Origin;

    FVector CameraLocation = MeshOrigin + FVector(MeshBounds.GetMax() * 2.0f); // Adjust distance as needed
    SceneCapture->SetWorldLocationAndRotation(CameraLocation, FRotator(-30.f, 0.f, 0.f)); // Adjust rotation as needed
}

UTextureRenderTarget2D* UDaRenderUtilLibrary::GenerateThumbnailWithRenderTarget(UStaticMeshComponent* MeshComp, UObject* WorldContextObject)
{
    if (!MeshComp || !WorldContextObject)
    {
        return nullptr;
    }

    // Create a render target
    UTextureRenderTarget2D* RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(
        WorldContextObject, 256, 256, ETextureRenderTargetFormat::RTF_RGBA8);

    if (!RenderTarget)
    {
        return nullptr;
    }

    // Create a temporary actor to hold the scene capture component
    AActor* TempActor = WorldContextObject->GetWorld()->SpawnActor<AActor>();
    USceneCaptureComponent2D* SceneCapture = NewObject<USceneCaptureComponent2D>(TempActor);
    SceneCapture->AttachToComponent(TempActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    SceneCapture->TextureTarget = RenderTarget;

    // Setup the scene capture
    SetupSceneCaptureForThumbnail(SceneCapture, MeshComp);

    // Trigger a one-time capture
    SceneCapture->CaptureScene();

    // Clean up the temporary actor
    TempActor->Destroy();

    return RenderTarget;
}

USlateBrushAsset* UDaRenderUtilLibrary::CreateSlateBrushFromRenderTarget(UTextureRenderTarget2D* RenderTarget)
{
    if (!RenderTarget)
    {
        return nullptr;
    }

    USlateBrushAsset* BrushAsset = NewObject<USlateBrushAsset>();
    BrushAsset->Brush.SetResourceObject(RenderTarget);
    BrushAsset->Brush.ImageSize = FVector2D(RenderTarget->SizeX, RenderTarget->SizeY);
    return BrushAsset;
}
