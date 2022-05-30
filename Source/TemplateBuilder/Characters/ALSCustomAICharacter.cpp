// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSCustomAICharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TemplateBuilder/GameModes/TemplateGameModeBase.h"
#include "TimerManager.h"
#include "TemplateBuilder/Health/HealthComponent.h"
#include "TemplateBuilder/Interactable/WeaponPickupBase.h"

#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "TemplateBuilder/AI/AIControllerBase.h"


AALSCustomAICharacter::AALSCustomAICharacter(const FObjectInitializer& ObjectInitializer) 
: AALSCustomCharacter (ObjectInitializer)
{

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	HealthBar->SetupAttachment(GetMesh());
	GetMesh()->OnComponentHit.AddDynamic(this, &AALSCustomAICharacter::OnHit);
	bIsNPC = true;
	HealthComponent->bHasShield = false;
	PickupReach = 150.0f;
}

void AALSCustomAICharacter::BeginPlay() 
{
    Super::BeginPlay();
}


void AALSCustomAICharacter::OnInteract_Implementation(AActor* Caller) 
{
    UE_LOG(LogTemp,Warning, TEXT("Interact with AI"));
}

void AALSCustomAICharacter::OnPickUp_Implementation(AActor* Caller) 
{
    
}

void AALSCustomAICharacter::StartFocus_Implementation() 
{
    if(IsDead() == false)
	{
		HealthBar->SetVisibility(true);
		GetWorldTimerManager().SetTimer(HealthBarTimer, this, &AALSCustomAICharacter::FaceHealthBarToPlayer, 0.2f, true, 0);		
	}
}

void AALSCustomAICharacter::EndFocus_Implementation() 
{
    if(IsDead() == false)
	{
		HealthBar->SetVisibility(false);
		GetWorldTimerManager().ClearTimer(HealthBarTimer);
	}
}



void AALSCustomAICharacter::TraceForward()
{
	FVector Location;
	FRotator Rotation;
	FHitResult Hit;
	GetActorEyesViewPoint(Location, Rotation);
	// GetController()->GetPlayerViewPoint(Location, Rotation);
	FVector StartPoint = Location;
	FVector EndPoint = StartPoint + (Rotation.Vector() * PickupReach);
	FCollisionQueryParams TraceParams;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	DrawDebugBox(GetWorld(), StartPoint, FVector(1,1,1), FColor::Orange, false, 1.0f);
	TArray<FHitResult> OutHits;
	// bool bHit = UKismetSystemLibrary::LineTraceSingle(this, StartPoint, EndPoint, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5),
	// true, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(this, StartPoint, (StartPoint * 1.01f), PickupReach,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5), true, ActorsToIgnore, EDrawDebugTrace::ForDuration,
		OutHits, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
	if(bHit)
	{
		for(int i = 1; i < OutHits.Num(); i++)
		{
			// UE_LOG(LogTemp,Warning,TEXT("AI TRACE HIT %i"), i);
			// UE_LOG(LogTemp,Warning,TEXT("%s"), *OutHits[i].Actor->GetName());
			IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(OutHits[i].Actor);
			if(InteractableActor)
			{
				// UE_LOG(LogTemp,Warning,TEXT("Interactable actor"));
			}
			DrawDebugBox(GetWorld(), OutHits[i].ImpactPoint, FVector(1,1,1), FColor::Orange, false, 1.0f);
		}
	}
}

//todo clean up Pickup, seperate Trace and regular pickups 
void AALSCustomAICharacter::PickupNearbyWeapon()
{
	FVector Location;
	FRotator Rotation;
	GetActorEyesViewPoint(Location, Rotation);
	Location = GetActorLocation();
	FVector StartPoint = Location;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<FHitResult> OutHits;
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(this, StartPoint, (StartPoint * 1.01f), PickupReach,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5), true, ActorsToIgnore, EDrawDebugTrace::ForDuration,
		OutHits, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
	if(bHit)
	{
		for(int i = 1; i < OutHits.Num(); i++)
		{
			IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(OutHits[i].GetActor());
			if(InteractableActor)
			{
				Crouch();
				// UE_LOG(LogTemp,Warning,TEXT("Interactable actor %s"), *OutHits[i].Actor->GetName());
				AWeaponPickupBase* WeaponPickup = Cast<AWeaponPickupBase>(OutHits[i].GetActor());
				if(WeaponPickup)
				{
					// UE_LOG(LogTemp,Warning,TEXT("Weapon Pickup"));
					InteractableActor->Execute_OnInteract(OutHits[i].GetActor(), this);
				}
				else
				{
					// UE_LOG(LogTemp,Warning,TEXT("Not Weapon"));
				}
			}
		}
	}
	
}

void AALSCustomAICharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if(NormalImpulse)
	//UE_LOG(LogTemp, Warning, TEXT("Hit"));
	//Cast To Character 
	//
}

void AALSCustomAICharacter::FaceHealthBarToPlayer() 
{
	if(IsDead())
	{
		GetWorldTimerManager().ClearTimer(HealthBarTimer);
		return;
	}
	APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	if(CameraManager)
	{
		FVector CameraLocation = CameraManager->GetCameraLocation();
		FVector HealthLocation = HealthBar->GetComponentLocation();
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(HealthLocation, CameraLocation);
		HealthBar->SetWorldRotation(NewRotation);
	}
}

void AALSCustomAICharacter::Death_Implementation() 
{
	Super::Death_Implementation();
	if(!DeathOnce)
	{
		DeathOnce = true;
		AAIControllerBase* AIController = Cast<AAIControllerBase>(GetController());
		if(AIController)
		{
			AIController->Death();
			// AIController->UnPossess();
		}
		MulticastDeath();
	}
}

void AALSCustomAICharacter::MulticastDeath_Implementation() 
{
	if(HealthBar)
	{
		HealthBar->DestroyComponent();
	}
}