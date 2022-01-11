// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSCustomPlayerCharacter.h"
#include "TemplateBuilder/Interactable/InteractableInterface.h"
#include "TemplateBuilder/Debug/DebugInterface.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "TemplateBuilder/Components/ShootingComponent.h"

AALSCustomPlayerCharacter::AALSCustomPlayerCharacter(const FObjectInitializer& ObjectInitializer) 
: AALSCustomCharacter (ObjectInitializer)
{
	bIsNPC = false;
	UseLength = 5000.0f;
}

void AALSCustomPlayerCharacter::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);
    if(!IsDead())
    {
        TraceForward();
    }
}

void AALSCustomPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) 
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("UseAction", IE_Pressed, this, &AALSCustomPlayerCharacter::Use);
	PlayerInputComponent->BindAction("AimAction", IE_Pressed, this, &AALSCustomPlayerCharacter::AimPressedAction);
	PlayerInputComponent->BindAction("AimAction", IE_Released, this, &AALSCustomPlayerCharacter::AimReleasedAction);
	PlayerInputComponent->BindAction("DebugModeAction", IE_Pressed, this, &AALSCustomPlayerCharacter::DebugCurrentWeapon);
	PlayerInputComponent->BindAction("PauseAction", IE_Pressed, this, &AALSCustomPlayerCharacter::PauseGame);
	// PlayerInputComponent->BindAction("ThrowWeaponAction", IE_Pressed, this, &AALSCustomPlayerCharacter::ThrowWeaponAction);
}

void AALSCustomPlayerCharacter::DebugCurrentWeapon() 
{
	if(!bDebuggingMode){bDebuggingMode = true;}
	else{bDebuggingMode = false;}
	IDebugInterface* WeaponDebugInterface = Cast<IDebugInterface>(Gun->GetChildActor());
	if(WeaponDebugInterface)
	{
		WeaponDebugInterface->Execute_EnableWeaponDebug(Gun->GetChildActor(), bDebuggingMode);
	}
	IDebugInterface* PlayerControllerDebugInterface = Cast<IDebugInterface>(GetController());
	if(PlayerControllerDebugInterface)
	{
		PlayerControllerDebugInterface->Execute_EnableControllerDebug(GetController(), bDebuggingMode);
	}
}

void AALSCustomPlayerCharacter::TraceForward_Implementation() 
{
	if(bIsAiming)
	{
		FVector Location;
		FRotator Rotation;
		FHitResult Hit;
		// if(HasAuthority())
		// {
		// 	//UE_LOG(LogTemp,Warning, TEXT("Server Trace"));
		// }
		GetController()->GetPlayerViewPoint(Location, Rotation);
		FVector StartPoint = Location;
		//if(bIsInFirstPerson)
		//{
		//	UseLength = UseLength * 2; //Multiplies it each time, needs to reset first
		//}
		FVector EndPoint = StartPoint + (Rotation.Vector() * UseLength);
		FCollisionQueryParams TraceParams;
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPoint, EndPoint, ECC_GameTraceChannel5, TraceParams);
		//if(bDebuggingMode)	{DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Orange, false, 0.2f );	}
		//UKismetSystemLibrary::LineTraceSingle(this, StartPoint, EndPoint, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5),false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true, FLinearColor::Red, FLinearColor::Green, 0.5f)
		if(bHit /*&& Interactable != this*/) //Needs to Not Hit Self
		{
			AActor* Interactable = Hit.GetActor();
			if(bDebuggingMode){	DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(1,1,1), FColor::Orange, false, 1.0f);}
			if(Interactable)
			{
				////////////////////////////
				if(Interactable != FocusedActor)
				{
					if(FocusedActor)
					{
						IInteractableInterface* Interface = Cast<IInteractableInterface>(FocusedActor);
						if (Interface!= nullptr)
						{
							Interface->Execute_EndFocus(FocusedActor);
						}
					}
					IInteractableInterface* Interface = Cast<IInteractableInterface>(Interactable);
					if(Interface != nullptr)
					{
						// If the Object: Has an interactable interface
						Interface->Execute_StartFocus(Interactable);
					}
					FocusedActor = Interactable;
				}
				////////////////////////////
			}
			else
			{
				if(FocusedActor)
				{
					IInteractableInterface* Interface = Cast<IInteractableInterface>(FocusedActor);
					if (Interface)
					{
						Interface->Execute_EndFocus(FocusedActor);
					}
				}
				FocusedActor = nullptr;
			}
		}
		else // if nothing is Hit, BUT there is a focused actor, End Focus and Set the actor to NULL
		{
			if(FocusedActor != nullptr)
			{
				IInteractableInterface* Interface = Cast<IInteractableInterface>(FocusedActor);
				if (Interface!= nullptr)
				{
					Interface->Execute_EndFocus(FocusedActor);
					FocusedActor = nullptr;
				}
			}
		}
	}
	else // If you stop aiming at the object, Stop focus
	{
		if(FocusedActor != nullptr)
		{
			IInteractableInterface* Interface = Cast<IInteractableInterface>(FocusedActor);
			if (Interface!= nullptr)
			{
				Interface->Execute_EndFocus(FocusedActor);
				FocusedActor = nullptr;
			}
		}
	}
}

void AALSCustomPlayerCharacter::ServerTraceForward_Implementation() 
{
	TraceForward();
}

void AALSCustomPlayerCharacter::AimPressedAction() 
{
	Super::AimPressedAction();
	StartAiming();
}

void AALSCustomPlayerCharacter::AimReleasedAction() 
{
	Super::AimReleasedAction();
	StopAiming();
}

void AALSCustomPlayerCharacter::StartAiming_Implementation() 
{
	//Any Default actions
}

void AALSCustomPlayerCharacter::StopAiming_Implementation() 
{
	//Any Default actions
}

void AALSCustomPlayerCharacter::Use() 
{
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerUse();
	}
	if(HasAuthority())
	{
		// UE_LOG(LogTemp,Warning, TEXT("Server USE"));
	}
	TraceForward();
	if(FocusedActor)
	{
		IInteractableInterface* Interface = Cast<IInteractableInterface>(FocusedActor);
		if(Interface)
		{
			Interface->Execute_OnInteract(FocusedActor, this);
		}
	}else
	{
		if(HasAuthority())
		{
			// UE_LOG(LogTemp,Warning, TEXT("NO Focused Actor"));
		}
	}
}

void AALSCustomPlayerCharacter::ServerUse_Implementation() 
{
	Use();
}

void AALSCustomPlayerCharacter::PauseGame_Implementation() 
{
	UGameplayStatics::SetGamePaused(this, true);
	//GetController()->bShowMouseCursor = true;
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeUIOnly());
}

void AALSCustomPlayerCharacter::ResumeGame() 
{
	UGameplayStatics::SetGamePaused(this, false);
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = false;
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeGameOnly());
}

void AALSCustomPlayerCharacter::ThrowWeaponAction() 
{
	ThrowWeaponEvent(ShootingComponent->GetCurrentWeaponData());
}

void AALSCustomPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AALSCustomPlayerCharacter, FocusedActor);
}


