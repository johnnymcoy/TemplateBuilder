// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WeaponSystemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

bool UWeaponSystemWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;
	return true;

}

void UWeaponSystemWidget::SetPlayerWeaponState(FPlayerWeaponState WeaponState)
{
	OnAutoModeChanged.Broadcast(WeaponState.bIsWeaponInAutoMode);
	OnIsAimingChanged.Broadcast(WeaponState.bIsAiming);
	OnIsReloadingChanged.Broadcast(WeaponState.bIsReloading);
	OnHolsteredChanged.Broadcast(WeaponState.bIsHolstered);
	OnHasGunChanged.Broadcast(WeaponState.bHasGun);
}

void UWeaponSystemWidget::UpdateWeapon(FWeaponData_T Weapon)
{
	CurrentAmmoTextBlock->SetText(FText::FromString(FString::FromInt(Weapon.CurrentAmmo)));
	CurrentTotalAmmoTextBlock->SetText(FText::FromString(FString::FromInt(Weapon.TotalAmmoCount)));
	CurrentWeaponNameTextBlock->SetText(FText::FromString(UEnum::GetValueAsString(Weapon.WeaponType)));
	CurrentWeaponThumbnailImage->SetBrushFromTexture(Weapon.GunThumbnail);
}

void UWeaponSystemWidget::UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo)
{
	CurrentAmmoTextBlock->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));
	CurrentTotalAmmoTextBlock->SetText(FText::FromString(FString::FromInt(TotalAmmo)));

}

void UWeaponSystemWidget::UpdateWeapons(TArray<FWeaponData_T> Weapons, int32 CurrentWeaponIndex)
{
	WeaponsInventory = Weapons;
	CurrentWeaponEquipped = CurrentWeaponIndex;
}


