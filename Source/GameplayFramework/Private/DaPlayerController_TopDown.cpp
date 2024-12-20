// Copyright Dream Awake Solutions LLC


#include "DaPlayerController_TopDown.h"

#include "CoreGameplayTags.h"
#include "DaInteractionComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"

ADaPlayerController_TopDown::ADaPlayerController_TopDown()
{
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	InputType = EGameplayInputType::GameAndCursor;
}

void ADaPlayerController_TopDown::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	AutoRun();
}

void ADaPlayerController_TopDown::SpawnClickFX()
{
	// Subclasses to implement
}

void ADaPlayerController_TopDown::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void ADaPlayerController_TopDown::AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(CoreGameplayTags::TAG_Input_Move))
	{
		if (UDaInteractionComponent* InteractionComponent = UDaInteractionComponent::FindInteractionComponent(GetPawn()))
		{
			bTargeting = InteractionComponent->GetFocusedActor() ? true : false;
			bAutoRunning = false;
		}
		
		// We're just interested in movement so dont let other abilities run
		return;
	}

	Super::AbilityInputTagPressed(Value, InputTag);
}

void ADaPlayerController_TopDown::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(CoreGameplayTags::TAG_Input_Move) && !bTargeting)
	{
		FollowedTime += GetWorld()->GetDeltaSeconds();

		if (UDaInteractionComponent* InteractionComponent = UDaInteractionComponent::FindInteractionComponent(GetPawn()))
		{
			FHitResult Hit = InteractionComponent->GetCursorHitResult();
			if (Hit.bBlockingHit) CachedDestination = Hit.ImpactPoint;
		}
		else
		{
			// fallback in case interaction component fails or doesnt exist for some reason
			FHitResult Hit;
			if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
			{
				CachedDestination = Hit.ImpactPoint;
			}
		}
		
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}

		// We're just interested in movement so dont let other abilities run
		return;
	}
	
	Super::AbilityInputTagHeld(InputTag);
}

void ADaPlayerController_TopDown::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(CoreGameplayTags::TAG_Input_Move))
	{
		if (!bTargeting)
		{
			const APawn* ControlledPawn = GetPawn();
			if (FollowedTime <= ShortPressThreshold && ControlledPawn)
			{
				if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
				{
					Spline->ClearSplinePoints();
					for (const FVector& PointLoc : NavPath->PathPoints)
					{
						Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
						DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
					}
					if (NavPath->PathPoints.Num() > 0) CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			
			}
			
		}
		else
		{
			if (UDaInteractionComponent* InteractionComponent = UDaInteractionComponent::FindInteractionComponent(GetPawn()))
			{
				InteractionComponent->PrimaryInteract();
			}
		}

		FollowedTime = 0.f;
		bTargeting = false;
		SpawnClickFX();
		
		// We're just interested in movement so dont let other abilities run
		return;
	} 
	
	Super::AbilityInputTagReleased(InputTag);
}

