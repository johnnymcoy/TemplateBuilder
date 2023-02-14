// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WeaponSystemWidget.h"

bool UWeaponSystemWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;
	// if (!ensure(HealthBar != nullptr)) return false;
	// if (!ensure(ShieldBar != nullptr)) return false;
	return true;

}

void UWeaponSystemWidget::SetHolstered(bool In_bIsHolstered)
{
	
}

void UWeaponSystemWidget::SetAutoMode(bool In_bIsInAutoMode)
{
	
}

void UWeaponSystemWidget::UpdateWeapon(FWeaponData_T Weapon)
{
	
}

void UWeaponSystemWidget::UpdateWeapons(TArray<FWeaponData_T> Weapons)
{
	
}


