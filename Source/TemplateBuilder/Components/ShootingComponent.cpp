// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingComponent.h"

#include "TemplateBuilder/Interactable/WeaponPickupBase.h"
#include "TemplateBuilder/Weapons/WeaponInterface.h"
#include "TemplateBuilder/HUD/WeaponWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

UShootingComponent::UShootingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
}

void UShootingComponent::BeginPlay()
{
	Super::BeginPlay();
	if(!CurrentWeaponData.IsValid())
	{
		//Todo remove all old bools 
		PlayerShootingStatus.bIsHolstered = true;
		// bIsHolstered = true;
	}
}

void UShootingComponent::UpdateWeaponHUD()
{
	GetOwner()->GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &UShootingComponent::DelayUpdateWeaponHUD, 0.001f, false);
}

void UShootingComponent::DelayUpdateWeaponHUD()
{
	// if(PlayerControlled)
	GetOwner()->GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeaponData = CurrentWeapon->Execute_GetWeaponData(GunChildActorReference->GetChildActor());
		if(PlayerShootingStatus.bPrimaryEquipped)
		{
			PrimaryWeaponData = CurrentWeaponData;
		}
		if(PlayerShootingStatus.bSecondaryEquipped)
		{
			SecondaryWeaponData = CurrentWeaponData;
		}
	}
	if(WeaponWidget != nullptr)
	{
		WeaponWidget->SetWeaponData(PrimaryWeaponData, SecondaryWeaponData, bPrimaryEquipped, bSecondaryEquipped);
	}
	//Todo Add hud into component?
	//Add crosshair functions
	//Current Weapon Get weapon data
	//Broadcast like health Component
}

void UShootingComponent::AimPressed(bool bRightShoulder)
{
	PlayerShootingStatus.bIsAiming = true;
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon && CurrentWeaponData.IsValid())
	{
		CurrentWeapon->Execute_MoveUMG(GunChildActorReference->GetChildActor(), !bRightShoulder);
		CurrentWeapon->Execute_FadeInUMG(GunChildActorReference->GetChildActor(), PlayerShootingStatus.bIsAiming);
		UE_LOG(LogTemp, Warning, TEXT("Fade UMG - ShootingComponent"));
	}
}

void UShootingComponent::AimReleased(bool bRightShoulder)
{
	PlayerShootingStatus.bIsAiming = false;
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon && CurrentWeaponData.IsValid())
	{
		CurrentWeapon->Execute_MoveUMG(GunChildActorReference->GetChildActor(), !bRightShoulder);
		CurrentWeapon->Execute_FadeInUMG(GunChildActorReference->GetChildActor(), PlayerShootingStatus.bIsAiming);
	}
}

void UShootingComponent::MoveUMG(bool bRightShoulder)
{
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon && CurrentWeaponData.IsValid())
	{
		CurrentWeapon->Execute_MoveUMG(GunChildActorReference->GetChildActor(), bRightShoulder);
	}
}

void UShootingComponent::SetupHUD()
{
	if(bIsNPC){return;};
	if(WeaponWidgetClass != nullptr)
	{
		WeaponWidget = CreateWidget<UWeaponWidget>(OwnerPlayerController, WeaponWidgetClass);
		if(WeaponWidget)
		{
			WeaponWidget->AddToViewport();
		}
	}else{UE_LOG(LogTemp, Error, TEXT("Add WeaponWidgetClass"));}
}

void UShootingComponent::ShootGun()
{
	if(CurrentWeaponData.CurrentAmmo <= 0){return;};
	//todo Accuracy in auto is same as first bullet
	//Accuracy sent from CustomCharacter;
	UE_LOG(LogTemp, Warning, TEXT("Accuracy: %f"), Accuracy);
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		if(PlayerShootingStatus.bIsAiming)
		{
			FVector InLocation;
			FRotator InRotation;
			if(bIsNPC)
			{
				GetOwner()->GetActorEyesViewPoint(InLocation, InRotation);
			}
			else
			{
				//TODO Get Reference to player camera through owner in begin play
				APlayerCameraManager* PlayerCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
				if(PlayerCamera)
				{
					InLocation = PlayerCamera->GetCameraLocation();
					InRotation = PlayerCamera->GetCameraRotation();
				}
			}
			//todo:Need to change name so it's more fittting From: GetTraceParams -> TO :ShootGun
			CurrentWeapon->Execute_GetTraceParams(GunChildActorReference->GetChildActor(), InLocation, InRotation, GetOwner(), Accuracy);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BlindFire"));
			//BlindFire Animation
			//Gun needs a blindfire Function that then goes into GetTraceParams
			//Gun interface also needs function
		}
		if(CurrentWeaponData.CurrentAmmo > 0)
		{
			Recoil();
		}
		UpdateWeaponHUD();
		//Update Ammo with a slight delay so there's time for the gun to fire
		// GetWorldTimerManager().SetTimer(HudUpdateHandle, this, &AALSCustomCharacter::UpdateWBPDelayed, 0.05f, false);
	}
}

void UShootingComponent::ValidateShootGun()
{
	if(CurrentWeaponData.CurrentAmmo <= 0){return;};
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		if(PlayerShootingStatus.bIsReloading && CurrentWeaponData.CurrentAmmo > 0){CancelReload();}
		if(CurrentWeapon->Execute_IsInAutoMode(GunChildActorReference->GetChildActor()))
		{
			GetOwner()->GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &UShootingComponent::ShootGun, (CurrentWeaponData.FireRate/ 200.f), true, 0.01f);
		}
		else
		{
			ShootGun();
		}
	}
}

void UShootingComponent::StopShootGun()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}

void UShootingComponent::Recoil()
{
	//todo need weaponData
	RecoilAmount = CurrentWeaponData.Recoil;
	// RecoilAmount = 0.5f;
	float RecoilTotal;
	//TODO Get rid of Magic number (4 and 8 multiplyer)
	if(PlayerShootingStatus.bIsAiming){RecoilTotal = (FMath::RandRange(RecoilAmount, (RecoilAmount * 4)) / Accuracy);}
	else{RecoilTotal = (FMath::RandRange(RecoilAmount, (RecoilAmount * 8)) / Accuracy);}
	UE_LOG(LogTemp, Warning, TEXT("Recoil %f - ShootingComponent"), RecoilTotal);
	//TODO Add pitchinput and recoilwbp
	//AddControllerPitchInput(RecoilTotal);
	//AddRecoilWBP(RecoilTotal);
}

void UShootingComponent::SwitchAutoMode()
{
	UE_LOG(LogTemp,Warning,TEXT("Swtich Auto Mode - ShootingComponent"));
	StopShootGun();
	//todo add HasAutoMode to weapondata
	//if hasautomode
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeapon->Execute_SwitchAutoMode(GunChildActorReference->GetChildActor());
	}
}

void UShootingComponent::Reload(UAnimMontage* ReloadAnimation)
{
	if(!PlayerShootingStatus.bIsReloading && CurrentWeaponData.IsValid())
	{
		IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
		if(CurrentWeapon)
		{
			if(CurrentWeaponData.CurrentAmmo < CurrentWeaponData.ClipSize && CurrentWeaponData.TotalAmmoCount > 0)
			{
				LastReloadAnimation = ReloadAnimation;
				float ReloadTime = MainAnimInstance->Montage_Play(ReloadAnimation, (1.0f / CurrentWeaponData.ReloadTime), EMontagePlayReturnType::Duration, 0.0f, true);
				if(ReloadTime <= 0.5)
				{
					ReloadTime = 1.0;
					UE_LOG(LogTemp,Warning, TEXT("ReloadTime is too slow, set to Default"));
					//todo: Have check for Server? make sure theres a default
				}
				CurrentWeapon->Execute_Reload(GunChildActorReference->GetChildActor(), ReloadTime);
				PlayerShootingStatus.bIsReloading = true;
				GetOwner()->GetWorldTimerManager().SetTimer(ReloadTimer, this, &UShootingComponent::ReloadDelay, (ReloadTime + 0.1f), false); //Offset so that the HUD updates properly
	// 			// if(GetLocalRole() < ROLE_Authority){ServerPlayMontageAnimation(GetReloadAnimation(CurrentWeaponData.WeaponType), (1.0f / CurrentWeaponData.ReloadTime), EMontagePlayReturnType::Duration, 0.0f, true);}
	// 			// else{MulticastPlayMontageAnimation(GetReloadAnimation(CurrentWeaponData.WeaponType), (1.0f / CurrentWeaponData.ReloadTime), EMontagePlayReturnType::Duration, 0.0f, true);}
			}
		}
	}
}

void UShootingComponent::ReloadDelay()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(ReloadTimer);
	if(PlayerShootingStatus.bIsReloading)
	{
		IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
		if(CurrentWeapon)
		{
			//todo move this to update hud
			CurrentWeaponData = CurrentWeapon->Execute_GetWeaponData(GunChildActorReference->GetChildActor());
			//if Player
			UpdateWeaponHUD();
		}
		PlayerShootingStatus.bIsReloading = false;
	}
}

void UShootingComponent::CancelReload()
{
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeapon->Execute_CancelReload(GunChildActorReference->GetChildActor());
	}
	PlayerShootingStatus.bIsReloading = false;
	MainAnimInstance->Montage_Stop(0.05f, LastReloadAnimation);
	// if(GetLocalRole() < ROLE_Authority){ServerStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	// else{MulticastStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	GetOwner()->GetWorldTimerManager().ClearTimer(ReloadTimer);
}

void UShootingComponent::PickupWeapon(FWeaponData WeaponToPickup)
{
	{
		// if(GetOwnerRole() < ROLE_Authority)
		// {
		//ServerPickupWeapon(WeaponData)
		// }
	}
	//todo Multiple Weapon Slots
	{
		//Don't have any weapons
		// if(CurrentWeaponListData.Num() == 0)
		// {
		// 	AddWeaponToList(WeaponToPickup, true);
		// 	EquipWeapon(WeaponToPickup);
		// }
		// else
		// {
		// 	for(int32 i = 0; i < CurrentWeaponListData.Num(); i++)
		// 	{
		// 		//Do we already have this weapon?
		// 		//Doesn;t Work, Checks First weapon pistol against first weapon holding rifle even if second weapon is pistol
		// 		
		// 		if(WeaponToPickup.WeaponType != CurrentWeaponListData[i].WeaponType)
		// 		{
		// 			AddWeaponToList(WeaponToPickup, true);
		// 			EquipWeapon(WeaponToPickup);
		// 		}
		// 		else if(WeaponToPickup.WeaponType == CurrentWeaponListData[i].WeaponType)
		// 		{
		// 			AddAmmo(WeaponToPickup);
		// 		}
		// 		UE_LOG(LogTemp,Warning,TEXT("For Loop %i"), i);
		// 	}
		// }
	}
	//todo if we have full ammo

	if(GetOwnerRole() == ROLE_Authority)
	{
		if(!WeaponToPickup.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid Pickup Weapon Data"));
			return;
		}
		//If we don't have any weapon
		//todo if(!PlayerShootingStatus.bHasGun)
		if(!PlayerShootingStatus.bHasGun)
		{
			UE_LOG(LogTemp, Warning, TEXT("First Pickup"));
			EquipWeapon(WeaponToPickup, true);
			return;
		}
		//Already have the weapon as a primary
		if(PrimaryWeaponData.WeaponClass == WeaponToPickup.WeaponClass)
		{
			AddAmmo(WeaponToPickup);
			// UE_LOG(LogTemp,Warning, TEXT("Pickup Primary ammo"));
			return;
		}
		///Check if we already have it as Secondary 
		if(SecondaryWeaponData.WeaponClass == WeaponToPickup.WeaponClass)
		{
			AddAmmo(WeaponToPickup, false);
			// UE_LOG(LogTemp,Warning, TEXT("Pickup Secondary ammo"));
			return;
		}
		////We don't have the weapon

		//We have a Primary out and no secondary weapon
		if(PlayerShootingStatus.bPrimaryEquipped && !SecondaryWeaponData.IsValid())
		{
			UE_LOG(LogTemp,Warning, TEXT("Pickup Secondary Weapon, don't have secondary currently"));
			//Pickup a secondary
			EquipWeapon(WeaponToPickup, false);
			return;
		}
		//Secondary but no Primary
		if(PlayerShootingStatus.bSecondaryEquipped && !PrimaryWeaponData.IsValid())
		{
			UE_LOG(LogTemp,Warning, TEXT("Pickup Primary Weapon, don't have Primary currently"));
			EquipWeapon(WeaponToPickup, true);
			return;
		}
		//Have Primary Weapon And secondary
		if(PrimaryWeaponData.IsValid() && SecondaryWeaponData.IsValid())
		{
			UE_LOG(LogTemp,Warning, TEXT("Two Weapons Equipped"));
			if(PlayerShootingStatus.bPrimaryEquipped)
			{
				ThrowWeapon(PrimaryWeaponData);
				EquipWeapon(WeaponToPickup, true);
			}
			if(PlayerShootingStatus.bSecondaryEquipped)
			{
				ThrowWeapon(SecondaryWeaponData);
				EquipWeapon(WeaponToPickup, false);
			}
		}
	}
	else
	{
		ServerPickupWeapon(WeaponToPickup);
	}
		// if(CurrentWeaponData.WeaponClass != WeaponToPickup.WeaponClass)
		// {
		// 	//if we already have a weapon equipped
		// 	if(CurrentWeaponData.IsValid())
		// 	{
		// 		// //todo If we Don't have a secondary
		// 		// if(SecondaryWeaponData.WeaponClass == EmptyWeaponData.WeaponClass)
		// 		// {
		// 		// 	SecondaryWeaponData = WeaponToPickup;
		// 		// }
		// 		ThrowWeapon(CurrentWeaponData);
		// 	}
		// 	EquipWeapon(WeaponToPickup);
		// }
		// //We already have this weapon
		// else
		// {
		// 	AddAmmo(WeaponToPickup);
		// }
	
}

void UShootingComponent::ServerPickupWeapon_Implementation(FWeaponData WeaponToPickup)
{
	PickupWeapon(WeaponToPickup);
}

bool UShootingComponent::ServerPickupWeapon_Validate(FWeaponData WeaponToPickup)
{
	return true;
}

void UShootingComponent::EquipWeapon(FWeaponData WeaponToEquip, bool bPrimaryWeapon)
{
	//todo not sure about < role
	// if(GetOwnerRole() < ROLE_Authority)
	// {
		// ServerEquipWeapon(WeaponData);
	// }
	PlayerShootingStatus.bHasGun = true;
	bHasGun = true;
	if(bPrimaryWeapon)
	{
		PrimaryWeaponData = WeaponToEquip;
		PlayerShootingStatus.bPrimaryEquipped = true;
		PlayerShootingStatus.bSecondaryEquipped = false;
	}
	else
	{
		SecondaryWeaponData = WeaponToEquip;
		PlayerShootingStatus.bSecondaryEquipped = true;
		PlayerShootingStatus.bPrimaryEquipped = false;
	}
	CurrentWeaponData = WeaponToEquip;
	UE_LOG(LogTemp, Warning, TEXT("Current Ammo: %i"), WeaponToEquip.CurrentAmmo);
	GunChildActorReference->SetChildActorClass(CurrentWeaponData.WeaponClass);
	//SetOverlayState(CurrentWeaponData.OverlayState);
	FName SocketName;
	if(CurrentWeaponData.bIsLeftHand)
	{SocketName = TEXT("VB LHS_ik_hand_gun");}
	else
	{SocketName = TEXT("VB RHS_ik_hand_gun");}
	GunChildActorReference->GetChildActor()->AttachToComponent(OwnerMesh,FAttachmentTransformRules::KeepWorldTransform, SocketName);
	GunChildActorReference->GetChildActor()->GetRootComponent()->SetRelativeLocation(CurrentWeaponData.WeaponOffset);
	GunChildActorReference->GetChildActor()->GetRootComponent()->SetRelativeRotation(CurrentWeaponData.RotationOffset);
	GunChildActorReference->GetChildActor()->SetOwner(GetOwner());
	PlayerShootingStatus.bIsHolstered = false;
	// ClearHeldObject();	
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeapon->Execute_SetWeaponData(GunChildActorReference->GetChildActor(), CurrentWeaponData);
		UpdateWeaponHUD();
		// PickupGunWBP(CurrentWeaponData);
	}
}

void UShootingComponent::ServerEquipWeapon_Implementation(FWeaponData WeaponToEquip, bool bPrimaryWeapon)
{
	EquipWeapon(WeaponToEquip, bPrimaryWeapon);
}

bool UShootingComponent::ServerEquipWeapon_Validate(FWeaponData WeaponToEquip, bool bPrimaryWeapon)
{
	return true;
}

void UShootingComponent::AddWeaponToList(FWeaponData SelectedWeapon, bool bAddWeapon)
{
	//for currentWeaponListData
	// Check if CurrentWeaponListData.WeaponClass == WeaponToAdd.WeaponClass
	// if bAddWeapon .add
	// else .Remove
	
	CurrentWeaponListData.Add(SelectedWeapon);
	if(CurrentWeaponListData.Num() > 0)
	{
		PlayerShootingStatus.bHasGun = true;
		bHasGun = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("List: %i"), CurrentWeaponListData.Num());

}

void UShootingComponent::SwapWeaponPressed()
{
	if(!bSwapWeaponPressed)
	{
		CancelReload();
		UWorld* World = GetWorld();
		if(World)
		{
			bSwapWeaponPressed = true;
			SwapWeaponActionPressedTime = World->GetTimeSeconds();
			GetOwner()->GetWorldTimerManager().SetTimer(OnWeaponSwapTimer, this, &UShootingComponent::HolsterWeapon, WeaponSwitchHoldTime, false);
		}
		else{UE_LOG(LogTemp,Error, TEXT("World Invalid on ShootingComponent %s"), *GetOwner()->GetName());}
	}
}

void UShootingComponent::SwapWeapon()
{
	bSwapWeaponPressed = false;
	// todo if(!PlayerShootingStatus.bHasGun){return;};
	if(!PlayerShootingStatus.bHasGun){return;}
	// if(SecondaryWeaponData.WeaponClass == EmptyWeaponData.WeaponClass)
	// {
	// 	//holster?
	// 	//or just return;
	// }
	//todo Fix all swap weapon with new weapon array
	{
		// //if we only have 1 weapon
		// if(CurrentWeaponListData.Num() == 1)
		// {
		// 	UE_LOG(LogTemp,Warning, TEXT("Single Weapon - Holster"));
		// 	HolsterWeapon();
		// }
		// //More than 1 weapon
		// if(CurrentWeaponListData.Num() > 1)
		// {
		// 	FWeaponData TempWeaponData;
		// 	
		// }
	}

	if(!PlayerShootingStatus.bIsHolstered)
	{
		if(PlayerShootingStatus.bPrimaryEquipped)
		{
			//Save WeaponData incase
			PrimaryWeaponData = CurrentWeaponData;
			//Is in Equip weapon now
			// bPrimaryEquipped = false;
			// bSecondaryEquipped = true;
			// CurrentWeaponData = SecondaryWeaponData;
			if(SecondaryWeaponData.IsValid())
			{
				UE_LOG(LogTemp,Warning,TEXT("Swap to Secondary"));
				EquipWeapon(SecondaryWeaponData, false);
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("NO Secondary - holster"));
				// HolsterWeapon();
			}
		}
		else
		{
			//Save Data
			SecondaryWeaponData = CurrentWeaponData;
			if(PrimaryWeaponData.IsValid())
			{
				UE_LOG(LogTemp,Warning,TEXT("Swap to Primary"));
				EquipWeapon(PrimaryWeaponData, true);
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("NO primary - holster"));
				// HolsterWeapon();
			}
		}
	}
	UpdateWeaponHUD();
}

void UShootingComponent::SwapWeaponReleased()
{
	UWorld* World = GetWorld();
	if(World)
	{
		//If we have only tapped the button (Pressed time is less than hold time)
		if(World->GetTimeSeconds() - SwapWeaponActionPressedTime <= WeaponSwitchHoldTime)
		{
			SwapWeapon();
			//Prevent Weapon Holster function
			GetOwner()->GetWorldTimerManager().ClearTimer(OnWeaponSwapTimer);
		}

	}
	else{UE_LOG(LogTemp,Error, TEXT("World Invalid on ShootingComponent %s"), *GetOwner()->GetName());}
}

void UShootingComponent::HolsterWeapon()
{
	bSwapWeaponPressed = false;
	GetOwner()->GetWorldTimerManager().ClearTimer(OnWeaponSwapTimer);
	if(!PlayerShootingStatus.bIsHolstered)
	{
		PlayerShootingStatus.bIsHolstered = true;
		UpdateWeaponHUD();
		//todo Setoverlay state
		// SetOverlayState(EALSOverlayState::Default);
		GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
		// ClearHeldObject();
		// if(GetOwnerRole() < ROLE_Authority)
		// {
		// ServerClearWeapon();
		// }
	}
	else
	{
		//Unholster
		PlayerShootingStatus.bIsHolstered = false;
		if(CurrentWeaponData.IsValid())
		{
			EquipWeapon(CurrentWeaponData, bPrimaryEquipped);
		}
		else
		{
			UE_LOG(LogTemp,Warning, TEXT("InvalidWeaponData (Holster)"));
		}
	}
}

void UShootingComponent::ThrowWeapon(FWeaponData WeaponToThrow)
{
	if(!WeaponToThrow.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon to drop"));
		return;
	}
	if(PlayerShootingStatus.bPrimaryEquipped)
	{
		PrimaryWeaponData = EmptyWeaponData;
		PlayerShootingStatus.bPrimaryEquipped = false;
	}
	if(PlayerShootingStatus.bSecondaryEquipped)
	{
		SecondaryWeaponData = EmptyWeaponData;
		PlayerShootingStatus.bSecondaryEquipped = false;
	}
	// if(bPrimaryEquipped)
	// {
	// 	bPrimaryEquipped = false;
	// 	PrimaryWeaponData = WeaponToThrow;
	// };
	// if(bSecondaryEquipped)
	// {
	// 	bSecondaryEquipped = false;
	// 	SecondaryWeaponData = WeaponToThrow;
	// 	PrimaryWeaponData = SecondaryWeaponData;
	// };
	CancelReload();
	DropWeapon(WeaponToThrow);
	ClearWeapon();
	GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
	CurrentWeaponData = EmptyWeaponData;
	UpdateWeaponHUD();
	// if(bPrimaryEquipped){bPrimaryEquipped = false;};
	// if(bSecondaryEquipped){bSecondaryEquipped = false;};
	//todo from owner
	//ServerDropGun(CurrentWeaponData, Location, Rotation, ThrowForce);
	// if(GetLocalRole() < ROLE_Authority)	{ServerClearWeapon();}
	// if(GetOwnerRole() < ROLE_Authority){ClearWeapon();}
	// Gun->SetChildActorClass(WeaponClassRef);
	// Gun->SetChildActorClass(EmptyWeaponData.WeaponClass);
	// WeaponDropped();
	// PickupGunWBP(CurrentWeaponData);
	//SetOverlayState(EALSOverlayState::Default);
	
}

void UShootingComponent::GetThrowStats(FTransform& OutTransform, FVector& OutThrowForce) const
{
	OutTransform.SetLocation(ThrowPoint->GetComponentLocation());
	//Random Rotation
	FRotator Rotation;
	Rotation.Yaw = UKismetMathLibrary::RandomFloatInRange(-180.0f, 180.0f);
	Rotation.Roll = UKismetMathLibrary::RandomFloatInRange(-180.0f, 180.0f);
	Rotation.Pitch = UKismetMathLibrary::RandomFloatInRange(-180.0f, 180.0f);
	OutTransform.SetRotation(Rotation.Quaternion());
	//Regular Scale
	OutTransform.SetScale3D(ThrowPoint->GetComponentScale());
	//Calculate Force
	FRotator ThrowRotation = ThrowPoint->GetComponentRotation();
	FVector ForwardVector = (FRotationMatrix(ThrowRotation).GetScaledAxis(EAxis::X));
	float OwnerVelocity =(GetOwner()->GetVelocity().Size() / 5);
	//Added in Accuracy to have greater control of throw and velocity for some momentum
	FVector FullThrowForce = ForwardVector * (PickupThrowIntensity / Accuracy + OwnerVelocity);
	OutThrowForce = FullThrowForce;
	UE_LOG(LogTemp,Warning, TEXT("Throw Force: %f , Velocity: %f"), OutThrowForce.Size(), GetOwner()->GetVelocity().Size());
}

//WeaponDropped
// CurrentWeaponData = EmptyWeaponData;
// PickupGunWBP(CurrentWeaponData);
// SetOverlayState(EALSOverlayState::Default);

//Server
void UShootingComponent::DropWeapon(FWeaponData SpawnWeaponData)
{
	FTransform Transform;
	FVector ThrowForce;
	GetThrowStats(Transform, ThrowForce);
	if(GunToSpawn != nullptr)
	{
		//removes weapon from list
		AddWeaponToList(SpawnWeaponData, false);
		FActorSpawnParameters SpawnParams;
		AWeaponPickupBase* DroppedWeaponRef = GetWorld()->SpawnActor<AWeaponPickupBase>(GunToSpawn, Transform.GetLocation(), Transform.GetRotation().Rotator(), SpawnParams);
		DroppedWeaponRef->WeaponDroppedEvent(SpawnWeaponData); //Set The pickup weapon stats
		DroppedWeaponRef->GunMesh->AddImpulse(ThrowForce, NAME_None, true);
	}
	else
	{
		UE_LOG(LogTemp,Error, TEXT("GunToSpawn not set"));
	}
	//Multicast
}

//Server
void UShootingComponent::ClearWeapon()
{
	GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
}

void UShootingComponent::AddAmmo(FWeaponData WeaponData, bool bIsPrimary)
{
	FWeaponData TempWeaponData;
	if(bIsPrimary)
	{
		TempWeaponData = PrimaryWeaponData;
		//Primary = WeaponData
		if(PlayerShootingStatus.bPrimaryEquipped)
		{
			//Add to current Weapon
		}
		UE_LOG(LogTemp,Warning, TEXT("Add Primary Ammo"));
	}
	else
	{
		TempWeaponData = SecondaryWeaponData;
		//Secondary = ammo
		if(!PlayerShootingStatus.bPrimaryEquipped)
		{
			//Add to current?
			//Double Check with some logs
		}
		UE_LOG(LogTemp,Warning, TEXT("Add Secondary Ammo"));
	}
	//If the total ammo is less than the max ammo && the total ammo + the pickup ammo is less than the max ammo, simply add them together
	if(TempWeaponData.TotalAmmoCount < TempWeaponData.MaxAmmo && (TempWeaponData.TotalAmmoCount + WeaponData.TotalAmmoCount) <= TempWeaponData.MaxAmmo)
	{
		TempWeaponData.TotalAmmoCount = TempWeaponData.TotalAmmoCount + WeaponData.TotalAmmoCount;
	}
	else if // If the total ammo is less than max, && the amount to pickup will end up more than the Max ammo
	(TempWeaponData.TotalAmmoCount < TempWeaponData.MaxAmmo && (TempWeaponData.TotalAmmoCount + WeaponData.TotalAmmoCount) > TempWeaponData.MaxAmmo)
	{
		TempWeaponData.TotalAmmoCount = TempWeaponData.MaxAmmo;
		//TODO::
		//Should not Destroy Actor
	}
	else // Should only cover if the ammo is Already Max 
	{
		//TODO::
		//Should not Destroy actor 
	}
	
	if(bIsPrimary)
	{
		PrimaryWeaponData = TempWeaponData;
		if(PlayerShootingStatus.bPrimaryEquipped)
		{
			CurrentWeaponData = PrimaryWeaponData;
		}
	}
	else
	{
		SecondaryWeaponData = TempWeaponData;
		if(!PlayerShootingStatus.bPrimaryEquipped)
		{
			CurrentWeaponData = SecondaryWeaponData;
		}
	}
	//Need to add in when picking up secondary ammo with primary equiped and vice 
	
	// ///Logic
	// //If the total ammo is less than the max ammo && the total ammo + the pickup ammo is less than the max ammo, simply add them together
	// if(CurrentWeaponData.TotalAmmoCount < CurrentWeaponData.MaxAmmo && (CurrentWeaponData.TotalAmmoCount + WeaponData.TotalAmmoCount) <= CurrentWeaponData.MaxAmmo)
	// {
	// 	CurrentWeaponData.TotalAmmoCount = CurrentWeaponData.TotalAmmoCount + WeaponData.TotalAmmoCount;
	// }
	// else if // If the total ammo is less than max, && the amount to pickup will end up more than the Max ammo
	// (CurrentWeaponData.TotalAmmoCount < CurrentWeaponData.MaxAmmo && (CurrentWeaponData.TotalAmmoCount + WeaponData.TotalAmmoCount) > CurrentWeaponData.MaxAmmo)
	// {
	// 	CurrentWeaponData.TotalAmmoCount = CurrentWeaponData.MaxAmmo;
	// 	//TODO::
	// 	//Should not Destroy Actor
	// }
	// else // Should only cover if the ammo is Already Max 
	// {
	// 	//TODO::
	// 	//Should not Destroy actor 
	// }
	
	//	UpdateWBP(CurrentWeaponData);
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeapon->Execute_SetWeaponData(GunChildActorReference->GetChildActor(), CurrentWeaponData);
		UpdateWeaponHUD();
		// PickupGunWBP(CurrentWeaponData);
	}
}

void UShootingComponent::Death()
{
	if(PlayerShootingStatus.bHasGun)
	{
		ThrowWeapon(CurrentWeaponData);
	// }
	// if(bHasGun)
	// {
		// //DropGun
		// //ServerDropGun
		// for(int i = 1; i > CurrentWeaponListData.Num(); i++)
		// {
		// 	ThrowWeapon(CurrentWeaponListData[i]);
		// }
		// ThrowWeapon(CurrentWeaponData);
	}
	GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
	SetActive(false);
}

void UShootingComponent::SetController(AController* Controller)
{
	if(bIsNPC)
	{
		OwnerAIController = Cast<AAIController>(OwnerController);
	}
	else
	{
		OwnerPlayerController = Cast<APlayerController>(OwnerController);
	}
}

void UShootingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UShootingComponent, GunChildActorReference);
	// DOREPLIFETIME(UShootingComponent, bIsHolstered);
	DOREPLIFETIME(UShootingComponent, CurrentWeaponData);
	DOREPLIFETIME(UShootingComponent,PlayerShootingStatus);

}
