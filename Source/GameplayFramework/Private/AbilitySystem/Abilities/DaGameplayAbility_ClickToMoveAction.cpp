// Copyright Dream Awake Solutions LLC


#include "AbilitySystem/Abilities/DaGameplayAbility_ClickToMoveAction.h"

#include "CoreGameplayTags.h"
#include "DaCharacter.h"
#include "DaInteractionComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/Tasks/DaAbilityTask_OnTick.h"
#include "Components/SplineComponent.h"

void UDaGameplayAbility_ClickToMoveAction::OnTriggeredInputAction(const FInputActionValue& Value)
{
	if (ADaCharacter* PlayerCharacter = Cast<ADaCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (APlayerController* PlayerController = CastChecked<APlayerController>(PlayerCharacter->Controller))
		{
			// Only handle movement if we're not targetting anything, otherwise same input tag is used for moving
			// Let other abilities run if we are targeting
			// if (!InputTag for this || bTargeting) -> Activate target actor ability
			
			if (!bTargeting)
			{
				FollowTime += GetWorld()->GetDeltaSeconds();
		
				FHitResult Hit;
				if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit))
				{
					CachedDestination = Hit.ImpactPoint;
				}

				const FVector WorldDirection = (CachedDestination - PlayerCharacter->GetActorLocation()).GetSafeNormal();
				PlayerCharacter->AddMovementInput(WorldDirection);
			}
		}
	}
}

void UDaGameplayAbility_ClickToMoveAction::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if (ADaCharacter* PlayerCharacter = Cast<ADaCharacter>(GetAvatarActorFromActorInfo()))
	{
		UDaInteractionComponent* InteractionComponent = PlayerCharacter->GetComponentByClass<UDaInteractionComponent>();
		if (InteractionComponent)
		{
			bTargeting = InteractionComponent->GetFocusedActor() ? true : false;
			bAutoRunning = false;
		}
	}
}

void UDaGameplayAbility_ClickToMoveAction::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ADaCharacter* PlayerCharacter = Cast<ADaCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (!bTargeting)
		{
			if (FollowTime <= ShortPressThreshold)
			{
				if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, PlayerCharacter->GetActorLocation(), CachedDestination))
				{
					Spline->ClearSplinePoints();
					for (const FVector& PointLoc: NavPath->PathPoints)
					{
						Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
						DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
					}
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
			FollowTime = 0.f;
			bTargeting = false;
		}
	}
	
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UDaGameplayAbility_ClickToMoveAction::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	TickTask = UDaAbilityTask_OnTick::AbilityTaskOnTick(this, "ClickToMoveReleaseTickTask");
	if (TickTask)
	{
		// FIXME: Task immediatly ends and Tick Never runs... Why?
		TickTask->OnTick.AddDynamic(this, &ThisClass::AutoRun);
		TickTask->Activate();
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UDaGameplayAbility_ClickToMoveAction::AutoRun(float Seconds)
{
	if (!bAutoRunning) return;
	if (ADaCharacter* PlayerCharacter = Cast<ADaCharacter>(GetAvatarActorFromActorInfo()))
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(PlayerCharacter->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		PlayerCharacter->AddMovementInput(Direction);
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void UDaGameplayAbility_ClickToMoveAction::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UDaGameplayAbility_ClickToMoveAction::UDaGameplayAbility_ClickToMoveAction(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
 	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

	StartupActivationTag = CoreGameplayTags::TAG_Input_Move;
	
}
