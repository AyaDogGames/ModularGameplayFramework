// Copyright Dream Awake Solutions LLC

#pragma once

#include "CoreMinimal.h"
#include "DaAttributeComponent.h"
#include "GameFramework/HUD.h"
#include "DaHUD.generated.h"

class UDaAttributeComponent;
class UAbilitySystemComponent;
class UDaOverlayWidgetController;
class UDaUserWidgetBase;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class GAMEPLAYFRAMEWORK_API ADaHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<UDaUserWidgetBase> OverlayWidget;

	UDaOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UDaAttributeComponent* AC);

	void RemoveOverlay();
	
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDaUserWidgetBase> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UDaOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDaOverlayWidgetController> OverlayWidgetControllerClass;
	
};
