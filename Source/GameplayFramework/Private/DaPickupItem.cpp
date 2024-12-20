// Copyright Dream Awake Solutions LLC


#include "DaPickupItem.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/DaAbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Util/ColorConstants.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADaPickupItem::ADaPickupItem()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComp");
	BaseMeshComp->SetupAttachment(RootComponent);
	BaseMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AbilitySystemComponent = CreateDefaultSubobject<UDaAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	bShouldRespawn = true;
	bShouldRespawnOnDeath = false;
	RespawnDelay = 10.0f;
	bIsActive = true;

	bUseDefaultHitFlash = true;
	HitFlashTimeParamName = "HitFlashTime";
	HitFlashColorParamName = "HitFlashColor";

	HitFlashColor = FVector(UE::Geometry::LinearColors::Purple3f());
	
	bReplicates = true;
}


void ADaPickupItem::Interact_Implementation(APawn* InstigatorPawn)
{
	// Derived classes to implement
}

FText ADaPickupItem::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	// Derived classes to implement
	return FText::GetEmpty();
}

void ADaPickupItem::HighlightActor_Implementation()
{
	bHighlighted = true;
}

void ADaPickupItem::UnHighlightActor_Implementation()
{
	bHighlighted = false;
}

UAbilitySystemComponent* ADaPickupItem::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
}

void ADaPickupItem::MulticastActiveStateChanged_Implementation(AActor* InstigatorActor, bool NewState)
{
	if (!NewState)
	{
		if (bUseDefaultHitFlash)
		{
			BaseMeshComp->SetScalarParameterValueOnMaterials(HitFlashTimeParamName, GetWorld()->TimeSeconds);
			BaseMeshComp->SetVectorParameterValueOnMaterials(HitFlashColorParamName, HitFlashColor);
		}
		
		// disable for a time, then re-enable if requested
		FTimerHandle TimerHandle_DelayedHide;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "FadeMesh", InstigatorActor);

		GetWorldTimerManager().SetTimer(TimerHandle_DelayedHide, Delegate, 0.25f, false );
	}
	else
	{
		SetActorEnableCollision(true);
		// Set visibility on root and all children
		RootComponent->SetVisibility(true, true);
		
		// broadcast to listeners
		OnActiveStateChanged.Broadcast(InstigatorActor, true);
	}
}

void ADaPickupItem::FadeMesh(AActor* InstigatorActor)
{
	SetActorEnableCollision(false);
	// Set visibility on root and all children
	RootComponent->SetVisibility(false, true);

	// broadcast to listeners
	OnActiveStateChanged.Broadcast(InstigatorActor, false);
}


void ADaPickupItem::SetItemState(APawn* InstigatorPawn, bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	MulticastActiveStateChanged(InstigatorPawn, bNewIsActive);
}

void ADaPickupItem::BeginPlay()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	Super::BeginPlay();
}

void ADaPickupItem::HideAndCooldownItem(APawn* InstigatorPawn)
{
	SetItemState(InstigatorPawn, false);

	if (bShouldRespawnOnDeath)
	{
		Cast<ACharacter>(InstigatorPawn)->GetController()->OnPossessedPawnChanged.AddDynamic(this, &ADaPickupItem::OnPlayerRespawn);
	}
	// destroy if this wont respawn
	else if (bShouldRespawn)
	{
		// start a 10 second timer to re-enable item if its still available to be picked up
		FTimerHandle TimerHandle_DelayedActivate;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ShowItem", InstigatorPawn);
		GetWorldTimerManager().SetTimer(TimerHandle_DelayedActivate, Delegate, RespawnDelay, false );
	}
	else
	{
		Destroy();
	}
}

void ADaPickupItem::OnPlayerRespawn(APawn* OldPawn, APawn* NewPawn)
{
	// start a 10 second timer to re-enable item if its still available to be picked up
	FTimerHandle TimerHandle_DelayedActivate;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "ShowItem", NewPawn);
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedActivate, Delegate, RespawnDelay, false );
}

void ADaPickupItem::ShowItem(APawn* InstigatorPawn)
{
	SetItemState(InstigatorPawn, true);
}

void ADaPickupItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADaPickupItem, bIsActive);
}
