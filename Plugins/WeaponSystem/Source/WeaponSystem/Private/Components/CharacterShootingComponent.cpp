// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterShootingComponent.h"
#include "Interfaces/Weapon.h"

UCharacterShootingComponent::UCharacterShootingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UCharacterShootingComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// PlayerInputComponent->BindAction("ShootAction", IE_Pressed, this, &UCharacterShootingComponent::FirePressedAction);
	PlayerInputComponent->BindAction("ShootAction", IE_Released, this, &UCharacterShootingComponent::StopShootGun);
	PlayerInputComponent->BindAction("AimAction", IE_Pressed, this, &UCharacterShootingComponent::AimPressedAction);
	PlayerInputComponent->BindAction("AimAction", IE_Released, this, &UCharacterShootingComponent::AimReleasedAction);
	PlayerInputComponent->BindAction("ReloadAction", IE_Pressed, this, &UCharacterShootingComponent::Reload);
	PlayerInputComponent->BindAction("AutoModeAction", IE_Pressed, this, &UCharacterShootingComponent::SwitchAutoMode);
	PlayerInputComponent->BindAction("SwapWeaponAction", IE_Pressed, this, &UCharacterShootingComponent::SwapWeaponPressed);
	PlayerInputComponent->BindAction("SwapWeaponAction", IE_Released, this, &UCharacterShootingComponent::SwapWeaponReleased);

}

void UCharacterShootingComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetOwner() == nullptr){LogMissingPointer("GetOwner");return;}
	OwnerActor = GetOwner();	
	PlayerWeaponState.bIsHolstered = true;
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon)
	{
		PlayerWeaponState.bIsHolstered = !Weapon->GetWeaponData().IsValid();
	}
}
// void UCharacterShootingComponent::AimPressed(bool bRightShoulder)

void UCharacterShootingComponent::AimPressedAction()
{
	PlayerWeaponState.bIsAiming = true;
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Aim Pressed"));}
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon)
	{
		// Weapon->MoveUMG(!bRightShoulder);
		// Weapon->FadeInUMG(PlayerWeaponState.bIsAiming);
		// UE_LOG(LogTemp, Warning, TEXT("Fade UMG - ShootingComponent"));
	}
}

void UCharacterShootingComponent::AimReleasedAction()
{
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Aim Released"));}

	PlayerWeaponState.bIsAiming = false;
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon)
	{
		// Weapon->MoveUMG(!bRightShoulder);
		// Weapon->FadeInUMG(PlayerWeaponState.bIsAiming);
		// UE_LOG(LogTemp, Warning, TEXT("Fade UMG - ShootingComponent"));
	}
}

void UCharacterShootingComponent::Reload()
{
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Reload Pressed"));}

	// if(!PlayerWeaponState.bIsReloading && GetCurrentWeaponData().IsValid())
	// {
	// 	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	// 	if(Weapon)
	// 	{
	// 	// 	if(CurrentWeaponData.CurrentAmmo < CurrentWeaponData.ClipSize && CurrentWeaponData.TotalAmmoCount > 0)
	// 	// 	{
	// 	// 		LastReloadAnimation = ReloadAnimation;
	// 	// 		float ReloadTime = MainAnimInstance->Montage_Play(ReloadAnimation, (1.0f / CurrentWeaponData.ReloadTime), EMontagePlayReturnType::Duration, 0.0f, true);
	// 	// 		if(ReloadTime <= 0.5)
	// 	// 		{
	// 	// 			ReloadTime = 1.0;
	// 	// 			UE_LOG(LogTemp,Warning, TEXT("ReloadTime is too slow, set to Default"));
	// 	// 			//todo: Have check for Server? make sure theres a default
	// 	// 		}
	// 	// 		CurrentWeapon->Reload(ReloadTime);
	// 	// 		PlayerShootingStatus.bIsReloading = true;
	// 	// 		GetOwner()->GetWorldTimerManager().SetTimer(ReloadTimer, this, &UShootingComponent::ReloadDelay, (ReloadTime + 0.1f), false); //Offset so that the HUD updates properly
	// 	// 		// 			// if(GetLocalRole() < ROLE_Authority){ServerPlayMontageAnimation(GetReloadAnimation(CurrentWeaponData.WeaponType), (1.0f / CurrentWeaponData.ReloadTime), EMontagePlayReturnType::Duration, 0.0f, true);}
	// 	// 		// 			// else{MulticastPlayMontageAnimation(GetReloadAnimation(CurrentWeaponData.WeaponType), (1.0f / CurrentWeaponData.ReloadTime), EMontagePlayReturnType::Duration, 0.0f, true);}
	// 	// 	}
	// 	}
	// }

	// GetReloadAnimation(GetCurrentWeaponData().WeaponType);
	// GetOwnerAnimInstance()->Montage_Play();
}

void UCharacterShootingComponent::SwitchAutoMode()
{
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Switch Auto mode"));}

}

void UCharacterShootingComponent::ShootGun()
{
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Shoot Gun"));}

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
			OwnerActor->GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &UCharacterShootingComponent::PullTrigger, (Weapon->GetWeaponData().FireRate/ 200.f), true, 0.01f);
		}
		else
		{
			PullTrigger();
		}
		
	}
}
void UCharacterShootingComponent::StopShootGun()
{
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Stop Shooting Gun"));}

	OwnerActor->GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}


void UCharacterShootingComponent::PullTrigger()
{
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Pull Trigger"));}

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
				OwnerActor->GetActorEyesViewPoint(InLocation, InRotation);
			}
			else
			{
				if(GetOwnerPlayerController() == nullptr){LogMissingPointer("Player Controller");return;}

				APlayerCameraManager* PlayerCamera = GetOwnerPlayerController()->PlayerCameraManager;
				if(PlayerCamera)
				{
					InLocation = PlayerCamera->GetCameraLocation();
					InRotation = PlayerCamera->GetCameraRotation();
				}
			}
			Weapon->Fire(InLocation, InRotation, OwnerActor, CalculateAccuracy());
		}
		else
		{
			if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("BlindFire"));};
			Weapon->BlindFireWeapon();
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
	if(OwnerActor->GetActorTimeDilation() < 1)
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

	OwnerActor->GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}

void UCharacterShootingComponent::PickupWeapon(FWeaponData_T WeaponToPickup)
{
	// Is Weapon Valid?
	if(!WeaponToPickup.IsValid())
	{
		LogMissingPointer("Invalid Weapon data to Pickup");
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
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Swap Weapon Pressed"));}
}

void UCharacterShootingComponent::SwapWeaponReleased()
{
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Swap Weapon Released"));}
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

FWeaponData_T UCharacterShootingComponent::GetCurrentWeapon() const
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

