// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSCustomAICharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TemplateBuilder/GameModes/TemplateGameModeBase.h"
#include "TimerManager.h"
#include "TemplateBuilder/Health/HealthComponent.h"


AALSCustomAICharacter::AALSCustomAICharacter(const FObjectInitializer& ObjectInitializer) 
: AALSCustomCharacter (ObjectInitializer)
{

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	HealthBar->SetupAttachment(GetMesh());
	GetMesh()->OnComponentHit.AddDynamic(this, &AALSCustomAICharacter::OnHit);
	bIsNPC = true;
	HealthComponent->bHasShield = false;
}

void AALSCustomAICharacter::BeginPlay() 
{
    Super::BeginPlay();
	SetDesiredGait(EALSGait::Walking);
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

void AALSCustomAICharacter::Death_Implementation() 
{
	Super::Death_Implementation();
	if(!DeathOnce)
	{
		DeathOnce = true;
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
