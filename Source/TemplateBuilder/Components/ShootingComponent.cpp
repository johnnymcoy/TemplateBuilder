// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingComponent.h"

UShootingComponent::UShootingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UShootingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//Only Tick if owner has a weapon
	if(bHasGun)
	{
		
	}

}

void UShootingComponent::ShootGun()
{
	//If(CurrentWeapon)
}

