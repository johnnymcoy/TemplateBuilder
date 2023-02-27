// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterShootingComponent.h"
#include "Interfaces/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/WeaponPickup.h"
#include "Net/UnrealNetwork.h"

//- Custom log Category //
DEFINE_LOG_CATEGORY(LogWeaponSystem);

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
	PlayerInputComponent->BindAction("ThrowWeaponAction", IE_Released, this, &UCharacterShootingComponent::ThrowWeaponAction);
}

void UCharacterShootingComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetOwner() == nullptr){LogMissingPointer("GetOwner");return;}
	OwnerActor = GetOwner();	
	PlayerWeaponState.bIsHolstered = true;
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		PlayerWeaponState.bIsHolstered = !Weapon->GetWeaponData().IsValid();
	}
	//? Temp for testing
	// PlayerWeaponState.bIsHolstered = false;
	// OnWeaponEqiupped.Broadcast(WeaponInventory, CurrentWeaponIndex);
}

//-													//
//-						Aiming						//
//-													//

// void UCharacterShootingComponent::AimPressed(bool bRightShoulder)
void UCharacterShootingComponent::AimPressedAction()
{
	PlayerWeaponState.bIsAiming = true;
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Aim Pressed"));}
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		// Weapon->MoveUMG(!bRightShoulder);
		// Weapon->FadeInUMG(PlayerWeaponState.bIsAiming);
		// UE_LOG(LogWeaponSystem, Warning, TEXT("Fade UMG - ShootingComponent"));
	}
}

void UCharacterShootingComponent::AimReleasedAction()
{
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Aim Released"));}

	PlayerWeaponState.bIsAiming = false;
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		// Weapon->MoveUMG(!bRightShoulder);
		// Weapon->FadeInUMG(PlayerWeaponState.bIsAiming);
		// UE_LOG(LogWeaponSystem, Warning, TEXT("Fade UMG - ShootingComponent"));
	}
}

//-													//
//-						Reloading					//
//-													//

void UCharacterShootingComponent::Reload()
{
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Reload Pressed"));}
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		FWeaponData_T WeaponData = Weapon->GetWeaponData();
		//- if it Is it a valid weapon and if we're not already reloading
		if(!WeaponData.IsValid()){return;};
		if(PlayerWeaponState.bIsReloading){return;}
		//- If our ammo if less that how much can fit in a clip, and we still have ammo
		if(WeaponData.CurrentAmmo < WeaponData.ClipSize && WeaponData.TotalAmmoCount > 0)
		{
			//? LastReloadAnimation = ReloadAnimation;
			//! v1
			//! UAnimMontage* ReloadAnimation = GetReloadAnimation(WeaponData.WeaponType);
			//! LastReloadAnimation = ReloadAnimation;
			ReloadAnimation = GetReloadAnimation(WeaponData.WeaponType);

			float ReloadTime = GetOwnerAnimInstance()->Montage_Play(
				ReloadAnimation,
				(1.0f / WeaponData.ReloadTime), 
				EMontagePlayReturnType::Duration,
				0.0f,
				true);
			//- In case of reload time incorrectly set or 0 
			if(ReloadTime <= 0.5f)
			{
				ReloadTime = 1.0f;
				LogMissingPointer("Reload time is too slow or not set, defaulting to 1");
				//todo: Have check for Server? make sure theres a default
			}
			Weapon->Reload(ReloadTime);
			PlayerWeaponState.bIsReloading = true;
			OwnerActor->GetWorldTimerManager().SetTimer(
				ReloadTimerHandle,
				this,
				&UCharacterShootingComponent::ReloadDelay,
				(ReloadTime + 0.1f), //-Offset so that the HUD updates properly
				false);
			//? 	// if(GetLocalRole() < ROLE_Authority){ServerPlayMontageAnimation(GetReloadAnimation(CurrentWeaponData.WeaponType), (1.0f / CurrentWeaponData.ReloadTime), EMontagePlayReturnType::Duration, 0.0f, true);}
			// ?	// else{MulticastPlayMontageAnimation(GetReloadAnimation(CurrentWeaponData.WeaponType), (1.0f / CurrentWeaponData.ReloadTime), EMontagePlayReturnType::Duration, 0.0f, true);}

		}
		//? Testing HUD update
		// - Update HUD elements // 
		OnAmmoChanged.Broadcast(Weapon->GetWeaponData().CurrentAmmo, Weapon->GetWeaponData().TotalAmmoCount);
	}


	// ! V1
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
	// 	// 			UE_LOG(LogWeaponSystem,Warning, TEXT("ReloadTime is too slow, set to Default"));
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

void UCharacterShootingComponent::ReloadDelay()
{
	OwnerActor->GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	if(!PlayerWeaponState.bIsReloading){return;}
	PlayerWeaponState.bIsReloading = false;
	// - Update HUD elements //
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		OnAmmoChanged.Broadcast(Weapon->GetWeaponData().CurrentAmmo, Weapon->GetWeaponData().TotalAmmoCount);
	}

	//- Update Hud elements 
	// todo Move this to update hud
	
	// IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	// if(Weapon != nullptr)
	// {
	// 	// CurrentWeaponData = Weapon->GetWeaponData();
	// 	// if player
	// 	// UpdateWeaponHUD();
	// }
}

void UCharacterShootingComponent::CancelReload()
{
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		Weapon->CancelReload();
	}
	PlayerWeaponState.bIsReloading = false;
	GetOwnerAnimInstance()->Montage_Stop(AnimationBlendOutTime, ReloadAnimation);
	//  if(GetLocalRole() < ROLE_Authority){ServerStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	//  else{MulticastStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	// BUG CLient Doesn't work 

	OwnerActor->GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}

//-													//
//-						Shooting					//
//-													//

void UCharacterShootingComponent::ShootGun()
{
	//? // Run on Server
	if(GetOwnerRole() != ROLE_Authority)
	{
		ServerShootGun();
	}

	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		//-		Do We have ammo		//
		if(Weapon->IsClipEmpty()){return;}
		//-	If we're tactical reloading, Cancel it //
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
//? // Run on Server
void UCharacterShootingComponent::ServerShootGun_Implementation()
{
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		//-		Do We have ammo		//
		if(Weapon->IsClipEmpty()){return;}
		//-	If we're tactical reloading, Cancel it //
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
	if(GetOwnerRole() != ROLE_Authority)
	{
		ServerStopShootGun();
	}
	OwnerActor->GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}

void UCharacterShootingComponent::ServerStopShootGun_Implementation()
{
	OwnerActor->GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}

void UCharacterShootingComponent::PullTrigger()
{
	//todo Accuracy in auto is same as first bullet - Check in Weapon Framework
	
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		//- Do We have ammo //
		if(Weapon->IsClipEmpty()){return;}
		FVector InLocation;
		FRotator InRotation;
		//- Could move this to AI Shooting Component //
		if(GetIsNPC())
		{
			OwnerActor->GetActorEyesViewPoint(InLocation, InRotation);
		}
		//- Could move this to Player Shooting Component //
		else
		{
			// //? Replicate this? the server doesn't know the Player Controller? 
			if(GetOwnerRole() == ROLE_AutonomousProxy || GetOwnerRole() == ROLE_Authority)
			{
				//? The error? 
				if(GetOwnerRole() == ROLE_Authority && GetOwnerPlayerController() == nullptr){return;}
				
				if(GetOwnerPlayerController() == nullptr){LogMissingPointer("Player Controller, Pull Trigger");return;}
				const APlayerCameraManager* PlayerCamera = GetOwnerPlayerController()->PlayerCameraManager;
				if(PlayerCamera == nullptr){LogMissingPointer("Player Camera");return;}
				InLocation = PlayerCamera->GetCameraLocation();
				InRotation = PlayerCamera->GetCameraRotation();
			}
		}
		
		if(PlayerWeaponState.bIsAiming)
		{
			Weapon->Fire(InLocation, InRotation, OwnerActor, CalculateAccuracy());
		}
		else
		{
			if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("BlindFire"));};
			Weapon->BlindFireWeapon(CalculateAccuracy(), OwnerActor);
			// Blindfire Animation
			//Gun needs a blindfire Function that then goes into GetTraceParams
		}
		Recoil();
		// - Update HUD elements // 
		OnAmmoChanged.Broadcast(Weapon->GetWeaponData().CurrentAmmo, Weapon->GetWeaponData().TotalAmmoCount);
	}
}

//-													//
//-						Recoil						//
//-													//

void UCharacterShootingComponent::Recoil()
{
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		const float RecoilAmount = Weapon->GetWeaponData().Recoil;
		float RecoilTotal;
		// Todo get rid of magic number (4 & 8 multiplier)
		if(PlayerWeaponState.bIsAiming){RecoilTotal = (FMath::RandRange(RecoilAmount, (RecoilAmount * 4)) / Accuracy);}
		else{RecoilTotal = (FMath::RandRange(RecoilAmount, (RecoilAmount * 8)) / Accuracy);}
		
		OnBulletShot.Broadcast(RecoilTotal);
	}
}

//-													//
//-						Accuracy					//
//-													//

float UCharacterShootingComponent::CalculateAccuracy()
{
	float CalculatedAccuracy = Accuracy;
	//todo get the Stance from somewhere.. 
	// if(Stance == EALSStance::Crounching)
	{
		// CalculatedAccuracy = (CalculatedAccuracy * 1.5f);
	}
	//- Slow Mo //
	if(OwnerActor->GetActorTimeDilation() < 1)
	{
		CalculatedAccuracy = (CalculatedAccuracy * 2.0f);	
	}
	//- BlindFire //
	if(!PlayerWeaponState.bIsAiming) // BlindFire
	{
		CalculatedAccuracy = (CalculatedAccuracy * 0.5f);
	}
	//-  AI Shooting Component //
	if(GetIsNPC())
	{
		CalculatedAccuracy = (CalculatedAccuracy * 0.25f);
	}
	return CalculatedAccuracy;
}

void UCharacterShootingComponent::SwitchAutoMode()
{
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Switch Auto mode"));}
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		//- Does the weapon actually have an auto mode ?
		if(!Weapon->GetWeaponData().bHasAutoMode){return;}
		StopShootGun();
		Weapon->SwitchAutoMode();
		PlayerWeaponState.bIsWeaponInAutoMode = Weapon->GetWeaponData().bIsInAutoMode;
		// todo
		// UpdateWeaponHUD();
		//? Testing HUD update
		OnWeaponStateChanged.Broadcast(PlayerWeaponState);
	}
}

//-													//
//-					Weapon Pickup					//
//-													//

void UCharacterShootingComponent::PickupWeapon(FWeaponData_T WeaponToPickup, int32& RemainingAmmo)
{
	//-  Is Weapon Valid //
	if(!WeaponToPickup.IsValid()){LogMissingPointer("Invalid Weapon data to Pickup");return;}
	//? // Run on Server
	if(GetOwnerRole() != ROLE_Authority)
	{
		ServerPickupWeapon(WeaponToPickup, RemainingAmmo);
	}
	//-		If It's our first weapon pickup		// 
	if(!PlayerWeaponState.bHasGun && WeaponInventory.Num() == 0)
	{
		if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("First Pickup"));}
		AddWeaponToInventory(WeaponToPickup);
		EquipWeapon(WeaponToPickup);
		RemainingAmmo = 0;
		// bWeaponWeHave = false;
		ServerHideWeaponModel(false);
	}
	else
	{
		bool bHasWeaponOfType = false;
		int32 Index = 0;
		for(int32 i = 0; i < WeaponInventory.Num(); i++)
		{
			if(WeaponToPickup.WeaponType == WeaponInventory[i].WeaponType)
			{
				bHasWeaponOfType = true;
				Index = i;
			}
		}
		//-  Do we already have this weapon // 
		if(bHasWeaponOfType)
		{
			// bWeaponWeHave = true;
			RemainingAmmo = AddAmmo(WeaponToPickup.TotalAmmoCount, Index);
		}
		else
		{
			// bWeaponWeHave = false;
			// CurrentWeaponIndex = WeaponInventory.Num() - 1;
			AddWeaponToInventory(WeaponToPickup);
			RemainingAmmo = 0;
			//! if we just swap weapon, if we're at index 0, weapons can be duplicated
			// SwapWeapon();
		}
	}
	OnWeaponEquipped.Broadcast(WeaponInventory, CurrentWeaponIndex);
	OnWeaponStateChanged.Broadcast(PlayerWeaponState);
}

void UCharacterShootingComponent::ServerPickupWeapon_Implementation(FWeaponData_T WeaponToPickup, int32 RemainingAmmo)
{
	UE_LOG(LogWeaponSystem, Warning, TEXT("Server Pickup Weapon"));
	PickupWeapon(WeaponToPickup, RemainingAmmo);
}

// Change to retrun &bool and &index 
bool UCharacterShootingComponent::PlayerHasWeaponOfType(EWeaponName_T WeaponType)
{
	for(const FWeaponData_T& WeaponData : WeaponInventory)
	{
		if(WeaponData.WeaponType == WeaponType)
		{
			return true;
		}
	}
	return false;
}

//-													//
//-					Equip Weapon					//
//-													//

void UCharacterShootingComponent::EquipWeapon(FWeaponData_T WeaponToEquip)
{
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Equip Weapon"));}
	//todo not sure about < role
	// if(GetOwnerRole() != ROLE_Authority)
	// {
	// 	ServerEquipWeapon(WeaponToEquip);
	// }
	PlayerWeaponState.bHasGun = true;
	PlayerWeaponState.bIsHolstered = false;

	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		Weapon->SetWeaponData(WeaponToEquip);
		PlayerWeaponState.bIsWeaponInAutoMode = WeaponToEquip.bIsInAutoMode;
		// CurrentWeapon->GunChildActorReference->SetChildActorClass(CurrentWeaponData.WeaponClass);
		FName SocketName;
		if(WeaponToEquip.bIsLeftHand)
		{SocketName = TEXT("VB LHS_ik_hand_gun");}
		else
		{SocketName = TEXT("VB RHS_ik_hand_gun");}
		Weapon->SetWeaponMesh(WeaponToEquip.MeshForPickup);
		CurrentWeapon->AttachToComponent(GetOwnerMesh(), FAttachmentTransformRules::KeepWorldTransform, SocketName);
		CurrentWeapon->GetRootComponent()->SetRelativeLocation(WeaponToEquip.WeaponOffset);
		CurrentWeapon->GetRootComponent()->SetRelativeRotation(WeaponToEquip.RotationOffset);
		CurrentWeapon->SetOwner(GetOwner());

	}
	OnWeaponEquipped.Broadcast(WeaponInventory, CurrentWeaponIndex);
	OnWeaponStateChanged.Broadcast(PlayerWeaponState);
}

//-													//
//-						Add Ammo					//
//-													//

int32 UCharacterShootingComponent::AddAmmo(const int32 AmountToAdd, const int32 WeaponIndex)
{
	int32 Remainder = WeaponInventory[WeaponIndex].AddAmmo(AmountToAdd);
	//- If We're holding the weapon we need to add ammo to, update the guns ammo counter //
	if(CurrentWeaponIndex == WeaponIndex)
	{
		IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
		if(Weapon != nullptr)
		{
			Weapon->SetWeaponData(WeaponInventory[CurrentWeaponIndex]);
		}
	}
	return Remainder;
}

void UCharacterShootingComponent::AddWeaponToInventory(FWeaponData_T NewWeapon)
{
	// if(GetOwnerRole() > ROLE_Authority)
	// {
	// 	ServerAddWeaponToInventory(NewWeapon);
	// }
	WeaponInventory.Emplace(NewWeapon);
	if(WeaponInventory.Num() > 0)
	{
		PlayerWeaponState.bHasGun = true;
		if(!PlayerWeaponState.bIsHolstered)
		{
			ServerHideWeaponModel(false);
		}
	}
	OnWeaponEquipped.Broadcast(WeaponInventory, CurrentWeaponIndex);
}

void UCharacterShootingComponent::RemoveWeaponFromInventory(int32 WeaponToRemove)
{
	// if(GetOwnerRole() > ROLE_Authority)
	// {
	// 	ServerRemoveWeaponFromInventory(WeaponToRemove);
	// }
	if(WeaponInventory.Num() > 0)
	{
		if(WeaponInventory.IsValidIndex(WeaponToRemove))
		{
			WeaponInventory.RemoveAt(WeaponToRemove);
		}
	}
	if(WeaponInventory.Num() == 0)
	{
		PlayerWeaponState.bHasGun = false;
		PlayerWeaponState.bIsHolstered = true;
		ServerHideWeaponModel(true);
	}
	OnWeaponEquipped.Broadcast(WeaponInventory, CurrentWeaponIndex);
}

// void UCharacterShootingComponent::ServerAddWeaponToInventory_Implementation(FWeaponData_T NewWeapon)
// {
// 	WeaponInventory.Emplace(NewWeapon);
// 	if(WeaponInventory.Num() > 0)
// 	{
// 		PlayerWeaponState.bHasGun = true;
// 		if(!PlayerWeaponState.bIsHolstered)
// 		{
// 			ServerHideWeaponModel(false);
// 		}
// 	}
// 	OnWeaponEquipped.Broadcast(WeaponInventory, CurrentWeaponIndex);
// }
//
// void UCharacterShootingComponent::ServerRemoveWeaponFromInventory_Implementation(int32 WeaponToRemove)
// {
// 	if(WeaponInventory.Num() > 0)
// 	{
// 		if(WeaponInventory.IsValidIndex(WeaponToRemove))
// 		{
// 			WeaponInventory.RemoveAt(WeaponToRemove);
// 		}
// 	}
// 	if(WeaponInventory.Num() == 0)
// 	{
// 		PlayerWeaponState.bHasGun = false;
// 		PlayerWeaponState.bIsHolstered = true;
// 		ServerHideWeaponModel(true);
// 	}
// 	OnWeaponEquipped.Broadcast(WeaponInventory, CurrentWeaponIndex);
// }

//-													//
//-					Swap Weapons					//
//-													//

void UCharacterShootingComponent::SwapWeaponPressed()
{
	StopShootGun();
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Swap Weapon Pressed"));}
	if(bSwapWeaponPressed){return;}
	CancelReload();
	UWorld* World = GetWorld();
	if(World == nullptr){LogMissingPointer("UWorld");return;}
	
	bSwapWeaponPressed = true;
	SwapWeaponPressedTime = World->GetTimeSeconds();
	OwnerActor->GetWorldTimerManager().SetTimer(WeaponSwapTimerHandle, this, &UCharacterShootingComponent::HolsterWeapon, SwapWeaponHoldTime, false);
}

void UCharacterShootingComponent::SwapWeapon()
{
	bSwapWeaponPressed = false;
	if(!PlayerWeaponState.bHasGun){return;}
	
	//? Added for Weapon auto to be shown on HUD
	//! Doesn't work if holstered as CurrentWeapon will be null, Maybe try the using the Current Index and TArray
	// IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	// if(Weapon != nullptr)
	// {
	// 	PlayerWeaponState.bIsWeaponInAutoMode = Weapon->GetWeaponData().bIsInAutoMode;
	// }

	//! Problem code...
	// When swapping weapons the data is not being stored correctly 
	if(WeaponInventory.IsValidIndex(CurrentWeaponIndex))
	{
		// BUG swapping weapon while holstered will overtake weapons
		// BUG also Picking up will overtake weapons when on index less than total !!!!!!!!!!!!!!!!!!!
		//? Update the data of the weapon in our inventory before switching
		IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
		if(Weapon != nullptr)
		{
			WeaponInventory[CurrentWeaponIndex] = Weapon->GetWeaponData();
		}

		//v1
		// UpdateWeaponInventory();
		// FWeaponData_T WeaponData;
		// GetCurrentWeaponData(WeaponData);
		// WeaponInventory[CurrentWeaponIndex] = WeaponData;
	}
	// int32 LastWeaponIndex = CurrentWeaponIndex;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponInventory.Num();
	
	// IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	// if(Weapon != nullptr)
	// {
	// 	WeaponInventory[LastWeaponIndex] = Weapon->GetWeaponData();
	// }

	OnWeaponStateChanged.Broadcast(PlayerWeaponState);
	OnWeaponEquipped.Broadcast(WeaponInventory, CurrentWeaponIndex);
	//? Should still swap if Holstered, but don't equip
	bool bReholsterWeapon = PlayerWeaponState.bIsHolstered;
	EquipWeapon(WeaponInventory[CurrentWeaponIndex]);
	if(bReholsterWeapon)
	{
		HolsterWeapon();
	}
	if(bDebuggingMode){UE_LOG(LogWeaponSystem,Warning,TEXT("Current Weapon: %i"), CurrentWeaponIndex);}
}


void UCharacterShootingComponent::SwapWeaponReleased()
{
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Swap Weapon Released"));}
	UWorld* World = GetWorld();
	if(World == nullptr){LogMissingPointer("UWorld");return;}
	
	//- If we have only tapped the button (Pressed time is less than hold time)
	if(World->GetTimeSeconds() - SwapWeaponPressedTime <= SwapWeaponHoldTime)
	{
		SwapWeapon();
		//- Prevent from Holstering Weapon
		OwnerActor->GetWorldTimerManager().ClearTimer(WeaponSwapTimerHandle);
	}
}

//-													//
//-						Holster						//
//-													//

void UCharacterShootingComponent::HolsterWeapon()
{
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Holster Weapon"));}
	bSwapWeaponPressed = false;
	OwnerActor->GetWorldTimerManager().ClearTimer(WeaponSwapTimerHandle);
	if(!PlayerWeaponState.bIsHolstered)
	{
		//- Holster // 
		PlayerWeaponState.bIsHolstered = true;
		ServerHideWeaponModel(true);
		if(CurrentWeapon != nullptr)
		{
			CurrentWeapon->SetActorHiddenInGame(true);
		}
		// if(CurrentWeapon != nullptr)
		// {
		// 	CurrentWeapon->SetActorHiddenInGame(true);
		// }
		// todo bIsInAutoMode
		
		// SetChildActorClass(EmptyWeaponData.WeaponClass);
		//ClearHeldObject();
	}
	else
	{
		//- UnHolster // 
		if(WeaponInventory.Num() == 0){UE_LOG(LogWeaponSystem, Warning, TEXT("No Weapons in Inventory"));return;}
		ServerHideWeaponModel(false);
		PlayerWeaponState.bIsHolstered = false;
		if(CurrentWeapon != nullptr)
		{
			CurrentWeapon->SetActorHiddenInGame(false);
		}
		if(WeaponInventory[CurrentWeaponIndex].IsValid())
		{
			if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Unholster + Equip Weapon"));}
			EquipWeapon(WeaponInventory[CurrentWeaponIndex]);
			// EquipWeapon();
		}
		else{UE_LOG(LogTemp,Warning, TEXT("InvalidWeaponData (Holster)"));}
	}
	OnWeaponStateChanged.Broadcast(PlayerWeaponState);
}

//-													//
//-					Throw Weapon					//
//-													//

void UCharacterShootingComponent::ThrowWeaponAction()
{
	FWeaponData_T WeaponData;
	//? needs to check if valid, maybe also check Index
	UpdateWeaponInventory();
	const bool bValidWeapon = GetCurrentWeaponData(WeaponData);
	if(bValidWeapon)
	{
		ThrowWeapon(WeaponData);
	}
}

//? Does this need to have an input? - Maybe if we want to throw a certain weapon //
void UCharacterShootingComponent::ThrowWeapon(FWeaponData_T WeaponToThrow)
{
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Throw Weapon"));}
	if(!WeaponToThrow.IsValid()){LogMissingPointer("Weapon to throw Not Valid");return;}
	//? Moved above going to server function
	if(!PlayerWeaponState.bHasGun){return;}
	if(GetOwnerRole() != ROLE_Authority)
	{
		ServerThrowWeapon(WeaponToThrow);
		return;
	}
	CancelReload();
	DropWeapon(WeaponToThrow);
	bSwapWeaponPressed = false;

	//v2
	//! Problem code...
	// When swapping weapons the data is not being stored correctly 
	if(WeaponInventory.IsValidIndex(CurrentWeaponIndex))
	{
		// BUG swapping weapon while holstered will overtake weapons
		// BUG also Picking up will overtake weapons when on index less than total
		//? Update the data of the weapon in our inventory before switching
		// IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
		// if(Weapon != nullptr)
		// {
		// 	WeaponInventory[CurrentWeaponIndex] = Weapon->GetWeaponData();
		// }

		//v1
		// UpdateWeaponInventory();
		FWeaponData_T WeaponData;
		GetCurrentWeaponData(WeaponData);
		WeaponInventory[CurrentWeaponIndex] = WeaponData;
	}
	// int32 LastWeaponIndex = CurrentWeaponIndex;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponInventory.Num();
	
	// IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	// if(Weapon != nullptr)
	// {
	// 	WeaponInventory[LastWeaponIndex] = Weapon->GetWeaponData();
	// }

	OnWeaponStateChanged.Broadcast(PlayerWeaponState);
	OnWeaponEquipped.Broadcast(WeaponInventory, CurrentWeaponIndex);
	
	//? Should still swap if Holstered, but don't equip
	bool bReholsterWeapon = PlayerWeaponState.bIsHolstered;
	EquipWeapon(WeaponInventory[CurrentWeaponIndex]);
	if(bReholsterWeapon)
	{
		HolsterWeapon();
	}
	if(bDebuggingMode){UE_LOG(LogWeaponSystem,Warning,TEXT("Current Weapon: %i"), CurrentWeaponIndex);}

	//v1
	// SwapWeapon();
}

void UCharacterShootingComponent::ServerThrowWeapon_Implementation(FWeaponData_T WeaponToThrow)
{
	ThrowWeapon(WeaponToThrow);
}


void UCharacterShootingComponent::DropWeapon(FWeaponData_T WeaponToDrop)
{
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Drop Weapon"));}
	if(WeaponToSpawn == nullptr){LogMissingPointer("Weapon To Spawn");return;}
	FTransform Transform;
	FVector ThrowForce;
	GetThrowStats(Transform, ThrowForce);
	//? Check
	RemoveWeaponFromInventory(CurrentWeaponIndex);
	
	const FActorSpawnParameters SpawnParameters;
	AWeaponPickup* DroppedWeapon = GetWorld()->SpawnActor<AWeaponPickup>(WeaponToSpawn, Transform.GetLocation(), Transform.GetRotation().Rotator(), SpawnParameters);
	//- Set the WeaponData of the Spawned Actor // 
	DroppedWeapon->WeaponDroppedEvent(WeaponToDrop);
	DroppedWeapon->GunMesh->AddImpulse(ThrowForce, NAME_None, true);
	OnWeaponEquipped.Broadcast(WeaponInventory, CurrentWeaponIndex);
	OnWeaponStateChanged.Broadcast(PlayerWeaponState);
}

void UCharacterShootingComponent::GetThrowStats(FTransform& OutTransform, FVector& OutThrowForce) const
{
	OutTransform.SetLocation(ThrowPoint->GetComponentLocation());
	//- Regular Scale //
	OutTransform.SetScale3D(ThrowPoint->GetComponentScale());
	//- Random Rotation // 
	FRotator Rotation;
	Rotation.Yaw = UKismetMathLibrary::RandomFloatInRange(-180.0f, 180.0f);
	Rotation.Roll = UKismetMathLibrary::RandomFloatInRange(-180.0f, 180.0f);
	Rotation.Pitch = UKismetMathLibrary::RandomFloatInRange(-180.0f, 180.0f);
	OutTransform.SetRotation(Rotation.Quaternion());
	//- Calculate Force // 
	FRotator ThrowRotation = ThrowPoint->GetComponentRotation();
	FVector ForwardVector = (FRotationMatrix(ThrowRotation).GetScaledAxis(EAxis::X));
	float OwnerVelocity =(GetOwner()->GetVelocity().Size() / 5);
	//- Added in Accuracy to have greater control of throw and velocity for some momentum
	FVector FullThrowForce = ForwardVector * (PickupThrowIntensity / Accuracy + OwnerVelocity);
	OutThrowForce = FullThrowForce;
}

//-													//
//-				Hide the Weapon Model				//
//-													//

void UCharacterShootingComponent::ServerHideWeaponModel_Implementation(bool bHidden)
{
	if(CurrentWeapon != nullptr)
	{
		CurrentWeapon->SetActorHiddenInGame(bHidden);
	}
}

void UCharacterShootingComponent::UpdateWeaponInventory()
{
	if(!WeaponInventory.IsValidIndex(CurrentWeaponIndex)){return;}
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		WeaponInventory[CurrentWeaponIndex] = Weapon->GetWeaponData();
	}
}

// todo Test for bugs if null
bool UCharacterShootingComponent::GetCurrentWeaponData(FWeaponData_T& CurrentWeaponData) const
{
	//todo Add in remove const 
	//	UpdateWeaponInventory();
	//? v2
	if(WeaponInventory.IsValidIndex(CurrentWeaponIndex))
	{
		CurrentWeaponData = WeaponInventory[CurrentWeaponIndex];
		if(CurrentWeaponData.IsValid())
		{
			return true;
		}
		LogMissingPointer("WeaponData Not Valid");
		return false;
	}
	LogMissingPointer("Invalid Weapon index");
	return false;

	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		CurrentWeaponData = Weapon->GetWeaponData();
		return CurrentWeaponData.IsValid();
	}
	return false;
}

//-													//
//-						Death						//
//-													//

void UCharacterShootingComponent::Death()
{
	// if(PlayerWeaponState.bHasGun)
	// {
	// 	ThrowWeapon(GetCurrentWeapon());
	// }
	// // GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
	// SetActive(false);
}

//-													//
//-						Replication					//
//-													//

void UCharacterShootingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterShootingComponent, CurrentWeapon)
	DOREPLIFETIME(UCharacterShootingComponent, WeaponInventory);
	// DOREPLIFETIME(UCharacterShootingComponent, CurrentWeaponIndex);

	// DOREPLIFETIME_CONDITION(UCharacterShootingComponent, CurrentWeaponIndex, COND_OwnerOnly);
}
