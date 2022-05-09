// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingReplicationComponent.h"
#include "ShootingComponent.h"
#include "Net/UnrealNetwork.h"

UShootingReplicationComponent::UShootingReplicationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}


void UShootingReplicationComponent::BeginPlay()
{
	Super::BeginPlay();
	ShootingComponent = GetOwner()->FindComponentByClass<UShootingComponent>();
	if(ShootingComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShootingComponent not found on: %s "), *GetOwner()->GetName());
	}
}


void UShootingReplicationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UShootingReplicationComponent::ServerValidateShootGun_Implementation()
{
	ShootingComponent->ValidateShootGun();
}

bool UShootingReplicationComponent::ServerValidateShootGun_Validate()
{
	return true;
}

void UShootingReplicationComponent::ServerPickupWeapon_Implementation(FWeaponData WeaponToPickup)
{
	CurrentWeaponData = WeaponToPickup;
	ShootingComponent->PickupWeapon(WeaponToPickup);
	// if(GetIsHolstered())
	// {
	// 	UE_LOG(LogTemp,Warning,TEXT("Holstered"));
	// }
	
}

bool UShootingReplicationComponent::ServerPickupWeapon_Validate(FWeaponData WeaponToPickup)
{
	return true;
}

void UShootingReplicationComponent::GetLifetimeReplicatedProps( TArray<FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UShootingReplicationComponent, CurrentWeaponData);

}
