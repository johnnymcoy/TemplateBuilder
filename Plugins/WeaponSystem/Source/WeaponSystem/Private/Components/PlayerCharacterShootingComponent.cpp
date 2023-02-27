// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerCharacterShootingComponent.h"
#include "Widgets/WeaponSystemWidget.h"
#include "Widgets/CrosshairWidget.h"

UPlayerCharacterShootingComponent::UPlayerCharacterShootingComponent()
{
	//- Bind Parent Functions To Widgets // 
	OnAmmoChanged.AddDynamic(this, &UPlayerCharacterShootingComponent::AmmoChanged);
	OnWeaponEquipped.AddDynamic(this, &UPlayerCharacterShootingComponent::WeaponEquipped);
	OnWeaponStateChanged.AddDynamic(this, &UPlayerCharacterShootingComponent::WeaponStateChanged);
	//- Crosshair Widget Function Bind //
	OnBulletShot.AddDynamic(this, &UPlayerCharacterShootingComponent::BulletShot);
	SetIsReplicatedByDefault(true);
}

void UPlayerCharacterShootingComponent::ClientSetupWeaponWidget_Implementation()
{
	if(WeaponWidgetClass == nullptr){LogMissingPointer("Weapon Widget Class");return;}
	if(GetOwnerPlayerController() == nullptr){LogMissingPointer("Owner Player Controller"); return;}
	WeaponWidget = CreateWidget<UWeaponSystemWidget>(GetOwnerPlayerController(), WeaponWidgetClass);
	if(WeaponWidget == nullptr){LogMissingPointer("Weapon Widget Creation Failed"); return;}
	WeaponWidget->AddToViewport();	
}

void UPlayerCharacterShootingComponent::SetupWeaponWidget()
{
	ClientSetupWeaponWidget();
}

void UPlayerCharacterShootingComponent::SetupWeaponCrosshairWidget()
{
	ClientSetupWeaponCrosshairWidget();
}

void UPlayerCharacterShootingComponent::ClientSetupWeaponCrosshairWidget_Implementation()
{
	if(CrosshairWidgetClass == nullptr){LogMissingPointer("Crosshair Widget Class");return;}
	if(GetOwnerPlayerController() == nullptr){LogMissingPointer("Owner Player Controller"); return;}
	CrosshairWidget = CreateWidget<UCrosshairWidget>(GetOwnerPlayerController(), CrosshairWidgetClass);
	if(CrosshairWidget == nullptr){LogMissingPointer("Crosshair Widget Creation Failed"); return;}
	CrosshairWidget->AddToViewport();
}

void UPlayerCharacterShootingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerCharacterShootingComponent::UpdateCurrentWeapon()
{
	if(GetOwnerRole() != ROLE_Authority)
	{
		if(WeaponWidget == nullptr){LogMissingPointer("Weapon Widget"); return;}
		FWeaponData_T WeaponData;
		const bool bValidWeapon = GetCurrentWeaponData(WeaponData);
		if(!bValidWeapon){return;}
		WeaponWidget->UpdateWeapon(WeaponData);
	}
}

void UPlayerCharacterShootingComponent::AmmoChanged(int32 CurrentAmmo, int32 TotalAmmo)
{
	if(GetOwnerRole() != ROLE_Authority)
	{
		if(WeaponWidget == nullptr){LogMissingPointer("Weapon Widget"); return;}
		WeaponWidget->UpdateAmmo(CurrentAmmo,TotalAmmo);
	}
}

void UPlayerCharacterShootingComponent::WeaponEquipped(TArray<FWeaponData_T> Weapons, int32 in_CurrentWeaponIndex)
{
	//? OwnerRole removes errors, but the server player can't update there widget 
	if(GetOwnerRole() != ROLE_Authority)
	{
		if(WeaponWidget == nullptr){LogMissingPointer("Weapon Widget"); return;}
		WeaponWidget->UpdateWeapons(Weapons, in_CurrentWeaponIndex);
	}
}

void UPlayerCharacterShootingComponent::WeaponStateChanged(FPlayerWeaponState WeaponState)
{
	if(GetOwnerRole() != ROLE_Authority)
	{
		if(WeaponWidget == nullptr){LogMissingPointer("Weapon Widget"); return;}
		WeaponWidget->SetPlayerWeaponState(WeaponState);
	}
}

void UPlayerCharacterShootingComponent::BulletShot(float RecoilAmount)
{
	if(GetOwnerRole() != ROLE_Authority)
	{
		if(CrosshairWidget == nullptr){LogMissingPointer("Weapon Widget"); return;}
		CrosshairWidget->SetGunRecoil(RecoilAmount);
	}
}

