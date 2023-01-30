// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InteractableActor.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::OnInteract(AActor* Caller)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Interact"));
	ReceiveOnInteract(Caller);
}

void AInteractableActor::OnPickUp(AActor* Caller)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("OnPickUp"));
	ReceiveOnPickUp(Caller);
}

void AInteractableActor::StartFocus()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Startfocus"));
	ReceiveStartFocus();
}

void AInteractableActor::EndFocus()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("EndFocus"));
	ReceiveEndFocus();
}

