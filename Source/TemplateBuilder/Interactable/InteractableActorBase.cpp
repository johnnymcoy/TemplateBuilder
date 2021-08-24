// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorBase.h"

// Sets default values
AInteractableActorBase::AInteractableActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActorBase::OnInteract_Implementation(AActor* Caller) 
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Interact"));
}

void AInteractableActorBase::OnPickUp_Implementation(AActor* Caller) 
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("OnPickUp"));
}

void AInteractableActorBase::StartFocus_Implementation() 
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Startfocus"));
}

void AInteractableActorBase::EndFocus_Implementation() 
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("EndFocus"));
}

