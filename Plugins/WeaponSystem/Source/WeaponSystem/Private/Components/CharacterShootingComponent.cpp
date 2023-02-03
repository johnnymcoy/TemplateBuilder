// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterShootingComponent.h"
#include "Interfaces/Weapon.h"

UCharacterShootingComponent::UCharacterShootingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UCharacterShootingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	// if(Weapon->GetWeaponData()->IsValid())
	// {
	// 	PlayerWeaponState.bIsHolstered = true;
	// }
}

void UCharacterShootingComponent::AimPressed(bool bRightShoulder)
{
	PlayerWeaponState.bIsAiming = true;
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon)
	{
		// Weapon->MoveUMG(!bRightShoulder);
		// Weapon->FadeInUMG(PlayerWeaponState.bIsAiming);
		// UE_LOG(LogTemp, Warning, TEXT("Fade UMG - ShootingComponent"));
	}
}

void UCharacterShootingComponent::AimReleased(bool bRightShoulder)
{
	PlayerWeaponState.bIsAiming = false;
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon)
	{
		// Weapon->MoveUMG(!bRightShoulder);
		// Weapon->FadeInUMG(PlayerWeaponState.bIsAiming);
		// UE_LOG(LogTemp, Warning, TEXT("Fade UMG - ShootingComponent"));
	}
}

void UCharacterShootingComponent::ShootGun()
{
	// TODO Rewrite, the gun should do all this
	// if(CurrentWeaponData.CurrentAmmo <= 0){return;};
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon)
	{
		// Do We have ammo? 
		if(Weapon->IsClipEmpty()){return;}
		// If we're tactical reloading, Cancel it
		if(PlayerWeaponState.bIsReloading && !Weapon->IsClipEmpty()){CancelReload();};
		if(Weapon->IsInAutoMode())
		{
			//todo remove firerate / 200.0f
			GetOwner()->GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &UCharacterShootingComponent::PullTrigger, (Weapon->GetWeaponData().FireRate/ 200.f), true, 0.01f);
		}
		else
		{
			PullTrigger();
		}
		
	}
}
void UCharacterShootingComponent::StopShootGun()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}


void UCharacterShootingComponent::PullTrigger()
{
	//todo Accuracy in auto is same as first bullet
	// UE_LOG(LogTemp, Warning, TEXT("Accuracy: %f"), Accuracy);
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon)
	{
		// Do We have ammo? 
		if(Weapon->IsClipEmpty()){return;}
		if(PlayerWeaponState.bIsAiming)
		{
			FVector InLocation;
			FRotator InRotation;
			if(GetIsNPC())
			{
				GetOwner()->GetActorEyesViewPoint(InLocation, InRotation);
			}
			else
			{
				APlayerCameraManager* PlayerCamera = GetOwnerPlayerController()->PlayerCameraManager;
				if(PlayerCamera)
				{
					InLocation = PlayerCamera->GetCameraLocation();
					InRotation = PlayerCamera->GetCameraRotation();
				}
			}
			//todo:Need to change name so it's more fittting From: GetTraceParams -> TO :ShootGun
			Weapon->Fire(InLocation, InRotation, GetOwner(), CalculateAccuracy());
		}
		else
		{
			Weapon->BlindFireWeapon();
			// Debug UE_LOG(LogTemp, Warning, TEXT("BlindFire"));
			// Blindfire Animation
			//Gun needs a blindfire Function that then goes into GetTraceParams
			//Gun interface also needs function
		}
		Recoil();
		//todo Accuracy in auto is same as first bullet

	}
}

void UCharacterShootingComponent::Recoil()
{
	
}

float UCharacterShootingComponent::CalculateAccuracy()
{
	float CalculatedAccuracy = Accuracy;
	// if(Stance == EALSStance::Crounching)
	{
		// CalculatedAccuracy = (CalculatedAccuracy * 1.5f);
	}
	if(GetOwner()->GetActorTimeDilation() < 1)
	{
		// CalculatedAccuracy = (CalculatedAccuracy * 2.0f);	
	}
	if(!PlayerWeaponState.bIsAiming) // BlindFire
	{
		// CalculatedAccuracy = (CalculatedAccuracy * 0.5f);
	}
	if(GetIsNPC())
	{
		// CalculatedAccuracy = (CalculatedAccuracy * 0.25f);
	}

	return CalculatedAccuracy;
}


void UCharacterShootingComponent::ReloadDelay()
{
	
}

void UCharacterShootingComponent::CancelReload()
{
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon)
	{
		Weapon->CancelReload();
	}
	PlayerWeaponState.bIsReloading = false;
	// GetOwnerAnimInstance()->Montage_Stop(0.05f, LastReloadAnimation);
	//  if(GetLocalRole() < ROLE_Authority){ServerStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	//  else{MulticastStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	// BUG CLient Doesn't work 

	GetOwner()->GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}

void UCharacterShootingComponent::PickupWeapon(FWeaponData_T WeaponToPickup)
{
	// Is Weapon Valid?
	if(!WeaponToPickup.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Pickup Weapon Data"));
		return;
	}
	// Run on Server
	if(GetOwnerRole() != ROLE_Authority)
	{
		// ServerPickupWeapon(WeaponToPickup)
		return;
	}
	// Do we have any weapon?
	if(!PlayerWeaponState.bHasGun && WeaponInventory.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("First Pickup"));
		AddWeaponToInventory(WeaponToPickup);
		EquipWeapon(WeaponToPickup);
	}
	else
	{
		for(int32 i = 0; i < WeaponInventory.Num(); i++)
		{
			// UE_LOG(LogTemp,Warning,TEXT("For Loop %i"), i);
			// Do we already have this weapon
			// //Doesn;t Work, Checks First weapon pistol against first weapon holding rifle even if second weapon is pistol
			if(WeaponToPickup.WeaponType != WeaponInventory[i].WeaponType)
			{
				
			}
			else if(WeaponToPickup.WeaponType == WeaponInventory[i].WeaponType)
			{
				AddAmmo(WeaponToPickup.TotalAmmoCount, i);
				//Add Ammo
			}
		}
	}
}

void UCharacterShootingComponent::AddAmmo(const int32 AmountToAdd, const int32 WeaponIndex)
{
	WeaponInventory[WeaponIndex].AddAmmo(AmountToAdd);
	// Update WBP
	// IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	// if(CurrentWeapon)
	// {
	// 	CurrentWeapon->SetWeaponData(CurrentWeaponData);
	// 	UpdateWeaponHUD();
	// 	// PickupGunWBP(CurrentWeaponData);
	// }
}

void UCharacterShootingComponent::AddWeaponToInventory(FWeaponData_T NewWeapon)
{
	WeaponInventory.Add(NewWeapon);
}

void UCharacterShootingComponent::RemoveWeaponFromInventory(int32 WeaponToRemove)
{
	WeaponInventory.RemoveAt(WeaponToRemove);
}

void UCharacterShootingComponent::EquipWeapon(FWeaponData_T WeaponToEquip)
{
	
}


void UCharacterShootingComponent::SwapWeapon()
{
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponInventory.Num();
	// UE_LOG(LogTemp,Warning,TEXT("Current Weapon: %i"), CurrentWeaponIndex);
}

void UCharacterShootingComponent::SwapWeaponPressed()
{
	
}

void UCharacterShootingComponent::SwapWeaponReleased()
{
	
}

void UCharacterShootingComponent::HolsterWeapon()
{
	
}

void UCharacterShootingComponent::ThrowWeapon(FWeaponData_T WeaponToThrow)
{
	
}

void UCharacterShootingComponent::DropWeapon()
{
	FTransform Transform;
	FVector ThrowForce;
	// GetThrowStats(Transform, ThrowForce);
	// if(WeaponToSpawn == nullptr)
	// {
	// 	UE_LOG(LogTemp,Error, TEXT("Weapon To Spawn Not Set"));
	// 	return; 
	// }
	// RemoveWeaponFromInventory(GetCurrentWeapon());
	FActorSpawnParameters SpawnParameters;
	// AActor* DroppedWeapon = GetWorld()->SpawnActor<AActor>(WeaponToSpawn, Transform.GetLocation(), Transform.GetRotation(), SpawnParameters);
	
}


void UCharacterShootingComponent::ClearWeapon()
{
	
}

FWeaponData_T UCharacterShootingComponent::GetCurrentWeapon()
{
	return WeaponInventory[CurrentWeaponIndex];
}

////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////					Death					  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////

void UCharacterShootingComponent::Death()
{
	if(PlayerWeaponState.bHasGun)
	{
		ThrowWeapon(GetCurrentWeapon());
	}
	// GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
	SetActive(false);
}

////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////

