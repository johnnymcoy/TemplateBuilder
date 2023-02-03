// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponPickup.h"

void AWeaponPickup::OnInteract(AActor* Caller)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Interact"));
	ReceiveOnInteract(Caller);
}

void AWeaponPickup::OnPickUp(AActor* Caller)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("OnPickUp"));
	ReceiveOnPickUp(Caller);
}

void AWeaponPickup::StartFocus()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Startfocus"));
	ReceiveStartFocus();
}

void AWeaponPickup::EndFocus()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("EndFocus"));
	ReceiveEndFocus();
}
