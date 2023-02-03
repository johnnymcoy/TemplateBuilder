// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"

#include "Interfaces/InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	//Optimised Tick for Traceforward todo: check value
	if(bStartTraceOnBeginPlay)
	{
		GetOwner()->GetWorldTimerManager().SetTimer(TraceForwardTimerHandle, this, &UInteractionComponent::TraceForward, TraceTickRate, true);
	}
}

void UInteractionComponent::StartTraceForward()
{
	if(GetIsDead()){return;};
	GetOwner()->GetWorldTimerManager().SetTimer(TraceForwardTimerHandle, this, &UInteractionComponent::TraceForward, TraceTickRate, true);
}

void UInteractionComponent::StopTraceForward()
{
	// If you stop tracing at the object, Stop focus
	if(FocusedActor != nullptr)
	{
		IInteractionInterface* InteractableActor = Cast<IInteractionInterface>(FocusedActor);
		if(InteractableActor != nullptr)
		{
			InteractableActor->EndFocus();
			FocusedActor = nullptr;
		}
		//Reset Aim Assist
		// LookUpDownRate = LookRateDefault;
		// LookLeftRightRate = LookRateDefault;
	}
	
	
	// todo Probably need to reset Focused actor on Stop
	GetOwner()->GetWorldTimerManager().ClearTimer(TraceForwardTimerHandle);
}

// TODO Hard to read - split into separate functions 

void UInteractionComponent::TraceForward()
{
	if(GetIsDead()){return;};
	FVector Location;
	FRotator Rotation;
	FHitResult Hit;
	if(GetOwnerController() == nullptr)
	{
		UE_LOG(LogTemp,Error, TEXT("Controller not set in %s of %s"), ToCStr(GetName()), ToCStr(GetOwner()->GetName()));
		return;
	}
	GetOwnerController()->GetPlayerViewPoint(Location, Rotation);
	FVector StartPoint = Location;
	// todo Check: useLength (ifInFirstPerson) { EndPoint = (...) * (UseLength *2);}
	FVector EndPoint = StartPoint + (Rotation.Vector() * UseLength);
	FCollisionQueryParams TraceParams;
	// TODO ECC TRACE CHANNEL
	// bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPoint, EndPoint, ECC_GameTraceChannel5, TraceParams);
	// if(bDebuggingMode)	{DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Orange, false, 0.2f );	}
	TArray<AActor*> ActorsToIgnore;
	EDrawDebugTrace::Type DrawDebugType;
	if(bDebuggingMode){DrawDebugType = EDrawDebugTrace::ForDuration;}else{DrawDebugType = EDrawDebugTrace::None;}
	bool bHit = UKismetSystemLibrary::LineTraceSingle(this, StartPoint, EndPoint, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5),false, ActorsToIgnore, DrawDebugType, Hit, true, FLinearColor::Red, FLinearColor::Green, 0.5f);
	
	//TODO: Aim Assist
	if(bHit)
	{
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			//	Debugging
			// if(bDebuggingMode){	DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(1,1,1), FColor::Orange, false, 1.0f);}
			// AimAssist
			{
				// IAimAssistInterface* HitAimTarget = Cast<IAimAssistInterface>(HitActor->GetComponentByClass(UAimAssistTargetComponent::StaticClass()));
				// if(HitAimTarget)
				// {
				// 	//todo: add to character that has component, when dead deactivate
				// 	if(!HitAimTarget->bIsDead())
				// 	{
				// 		LookUpDownRate = (LookRateDefault * AimAssistLookRate);
				// 		LookLeftRightRate = (LookRateDefault * AimAssistLookRate);
				// 	}
				// }
				// else
				// {
				// 	LookUpDownRate = LookRateDefault;
				// 	LookLeftRightRate = LookRateDefault;
				// }
			}
			
			////////////////////////////
			if(HitActor != FocusedActor)
			{
				if(FocusedActor)
				{
					IInteractionInterface* InteractableActor = Cast<IInteractionInterface>(FocusedActor);
					if(InteractableActor != nullptr)
					{
						InteractableActor->EndFocus();
					}
				}
				IInteractionInterface* InteractableActor = Cast<IInteractionInterface>(HitActor);
				if(InteractableActor != nullptr)
				{
					// If the Object: Has an interactable interface
					InteractableActor->StartFocus();
				}
				FocusedActor = HitActor;
			}
			////////////////////////////
		}
		else
		{
			if(FocusedActor != nullptr)
			{
				IInteractionInterface* InteractableActor = Cast<IInteractionInterface>(FocusedActor);
				if(InteractableActor)
				{
					InteractableActor->EndFocus();
				}
			}
			FocusedActor = nullptr;
			// //Reset Aim Assist
			// LookUpDownRate = LookRateDefault;
			// LookLeftRightRate = LookRateDefault;
		}
	}
	// if nothing is Hit, BUT there is a focused actor, End Focus and Set the actor to NULL
	else
	{
		if(FocusedActor != nullptr)
		{
			IInteractionInterface* InteractableActor = Cast<IInteractionInterface>(FocusedActor);
			if (InteractableActor != nullptr)
			{
				InteractableActor->EndFocus();
				FocusedActor = nullptr;
			}
		}
	}
}


void UInteractionComponent::Use()
{
	StartTraceForward();
	if(FocusedActor != nullptr)
	{
		IInteractionInterface* InteractableActor = Cast<IInteractionInterface>(FocusedActor);
		if(InteractableActor != nullptr)
		{
			InteractableActor->OnInteract(GetOwner());
		}
	}
	// else
	// {
	// 	if(HasAuthority())
	// 	{
	// 		// UE_LOG(LogTemp,Warning, TEXT("NO Focused Actor"));
	// 	}
	// }
	StopTraceForward();
}
