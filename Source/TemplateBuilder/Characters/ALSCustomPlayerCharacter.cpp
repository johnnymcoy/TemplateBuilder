// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSCustomPlayerCharacter.h"
#include "TemplateBuilder/Interactable/InteractableInterface.h"
#include "TemplateBuilder/Debug/DebugInterface.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "TemplateBuilder/AimAssist/AimAssistInterface.h"
#include "TemplateBuilder/Components/AimAssistTargetComponent.h"
#include "TemplateBuilder/Components/ShootingComponent.h"
#include "TemplateBuilder/MenuSystem/MenuGameInstance.h"

AALSCustomPlayerCharacter::AALSCustomPlayerCharacter(const FObjectInitializer& ObjectInitializer) 
: AALSCustomCharacter (ObjectInitializer)
{
	bIsNPC = false;
	UseLength = 5000.0f;
}

void AALSCustomPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp,Warning,TEXT("Version 0.12.05"));
	//Optimised Tick for Traceforward todo: check value 
	GetWorldTimerManager().SetTimer(TraceForwardTimerHandle, this, &AALSCustomPlayerCharacter::TraceForward, 0.1f, true);
}


void AALSCustomPlayerCharacter::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);
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
		WeaponDebugInterface->EnableWeaponDebug(bDebuggingMode);
	}
}

void AALSCustomPlayerCharacter::TraceForward_Implementation() 
{
	if(IsDead()){return;};
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
		//TODO: Aim Assist
		if(bHit /*&& Interactable != this*/) //Needs to Not Hit Self
		{
			AActor* HitActor = Hit.GetActor();
			if(HitActor)
			{
				//	Debugging
				if(bDebuggingMode){	DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(1,1,1), FColor::Orange, false, 1.0f);}
				//	Aim Assist
				IAimAssistInterface* HitAimTarget = Cast<IAimAssistInterface>(HitActor->GetComponentByClass(UAimAssistTargetComponent::StaticClass()));
				if(HitAimTarget)
				{
					//todo: add to character that has component, when dead deactivate
					if(!HitAimTarget->bIsDead())
					{
						LookUpDownRate = (LookRateDefault * AimAssistLookRate);
						LookLeftRightRate = (LookRateDefault * AimAssistLookRate);
					}
				}
				else
				{
					LookUpDownRate = LookRateDefault;
					LookLeftRightRate = LookRateDefault;
				}
				////////////////////////////
				if(HitActor != FocusedActor)
				{
					if(FocusedActor)
					{
						IInteractableInterface* Interface = Cast<IInteractableInterface>(FocusedActor);
						if (Interface != nullptr)
						{
							Interface->EndFocus();
						}
					}
					IInteractableInterface* Interface = Cast<IInteractableInterface>(HitActor);
					if(Interface != nullptr)
					{
						// If the Object: Has an interactable interface
						Interface->StartFocus();
					}
					FocusedActor = HitActor;
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
						Interface->EndFocus();
					}
				}
				FocusedActor = nullptr;
				// //Reset Aim Assist
				LookUpDownRate = LookRateDefault;
				LookLeftRightRate = LookRateDefault;
			}
		}
		else // if nothing is Hit, BUT there is a focused actor, End Focus and Set the actor to NULL
		{
			if(FocusedActor != nullptr)
			{
				IInteractableInterface* Interface = Cast<IInteractableInterface>(FocusedActor);
				if (Interface!= nullptr)
				{
					Interface->EndFocus();
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
				Interface->EndFocus();
				FocusedActor = nullptr;
			}
		//Reset Aim Assist
		LookUpDownRate = LookRateDefault;
		LookLeftRightRate = LookRateDefault;
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
			Interface->OnInteract(this);
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
	UMenuGameInstance* GameInstance = Cast<UMenuGameInstance>(GetGameInstance());
	if(GameInstance && IsLocallyControlled())
	{
		GameInstance->LaunchInGameMenu();
	}
}

void AALSCustomPlayerCharacter::Death_Implementation()
{
	Super::Death_Implementation();
	if(OwnerPlayerController != nullptr)
	{
		FInputModeUIOnly InputModeData;
		OwnerPlayerController->bShowMouseCursor = true;
		OwnerPlayerController->SetInputMode(InputModeData);
		UMenuGameInstance* GameInstance = Cast<UMenuGameInstance>(GetGameInstance());
		if(GameInstance && IsLocallyControlled())
		{
			GameInstance->LaunchInGameMenu(true);
		}
	}
}

//todo Disabled due to bugs 
void AALSCustomPlayerCharacter::ThrowWeaponAction() 
{
	ThrowWeaponEvent(ShootingComponent->GetCurrentWeaponData());
}

void AALSCustomPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AALSCustomPlayerCharacter, FocusedActor);
}



