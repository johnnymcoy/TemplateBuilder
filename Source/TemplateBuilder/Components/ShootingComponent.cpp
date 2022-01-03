// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingComponent.h"

#include "TemplateBuilder/Interactable/WeaponPickupBase.h"
#include "TemplateBuilder/Weapons/WeaponInterface.h"
#include "Kismet/KismetMathLibrary.h"

UShootingComponent::UShootingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UShootingComponent::BeginPlay()
{
	Super::BeginPlay();
	if(!CurrentWeaponData.IsValid()){bIsHolstered = true;}
}

void UShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//Only Tick if owner has a weapon
	if(bHasGun)
	{
		
	}
}

void UShootingComponent::UpdateWeaponHUD()
{
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeaponData = CurrentWeapon->Execute_GetWeaponData(GunChildActorReference->GetChildActor());
	}
	//if PlayerControlled
	//Todo weapon HUD function 
	//Current Weapon Get weapon data
	//Broadcast like health Component
}

void UShootingComponent::AimPressed(bool bRightShoulder)
{
	bIsAiming = true;
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon && CurrentWeaponData.IsValid())
	{
		CurrentWeapon->Execute_MoveUMG(GunChildActorReference->GetChildActor(), !bRightShoulder);
		CurrentWeapon->Execute_FadeInUMG(GunChildActorReference->GetChildActor(), bIsAiming);
		UE_LOG(LogTemp, Warning, TEXT("Fade UMG - ShootingComponent"));
	}
}

void UShootingComponent::AimReleased(bool bRightShoulder)
{
	bIsAiming = false;
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon && CurrentWeaponData.IsValid())
	{
		CurrentWeapon->Execute_MoveUMG(GunChildActorReference->GetChildActor(), !bRightShoulder);
		CurrentWeapon->Execute_FadeInUMG(GunChildActorReference->GetChildActor(), bIsAiming);
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

void UShootingComponent::ShootGun()
{
	//todo Do this is constructor?
	UE_LOG(LogTemp,Warning,TEXT("Accuracy %f ShootingComponent"), Accuracy);
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		FVector InLocation;
		FRotator InRotation;
		//if NPC?
		//TODO Get Reference to player camera through owner in begin play
		APlayerCameraManager* PlayerCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		if(PlayerCamera)
		{
			InLocation = PlayerCamera->GetCameraLocation();
			InRotation = PlayerCamera->GetCameraRotation();
		}
		// CalculateAccuracy();
		CurrentWeapon->Execute_GetTraceParams(GunChildActorReference->GetChildActor(), InLocation, InRotation, GetOwner(), Accuracy);
		//todo remove
		Recoil();
		if(CurrentWeaponData.CurrentAmmo > 0)
		{
			Recoil();
		}
		UpdateWeaponHUD();
		//UpdateHUD
	}
}

void UShootingComponent::ValidateShootGun()
{
	if(!bIsHolstered)
	{
		IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
		if(CurrentWeapon)
		{
			if(bIsReloading && CurrentWeaponData.CurrentAmmo > 0){CancelReload();}
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
	if(bIsAiming){RecoilTotal = (FMath::RandRange(RecoilAmount, (RecoilAmount * 4)) / Accuracy);}
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
	if(!bIsReloading && CurrentWeaponData.IsValid())
	{
		IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
		if(CurrentWeapon)
		{
			if(CurrentWeaponData.CurrentAmmo < CurrentWeaponData.ClipSize && CurrentWeaponData.TotalAmmoCount > 0)
			{
				LastReloadAnimation = ReloadAnimation;
				float ReloadTime = MainAnimInstance->Montage_Play(ReloadAnimation, (1.0f / CurrentWeaponData.ReloadTime), EMontagePlayReturnType::Duration, 0.0f, true);
				CurrentWeapon->Execute_Reload(GunChildActorReference->GetChildActor(), ReloadTime);
				bIsReloading = true;
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
	if(bIsReloading)
	{
		IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
		if(CurrentWeapon)
		{
			//todo move this to update hud
			CurrentWeaponData = CurrentWeapon->Execute_GetWeaponData(GunChildActorReference->GetChildActor());
			UpdateWeaponHUD();
		}
		bIsReloading = false;
	}
}

void UShootingComponent::CancelReload()
{
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeapon->Execute_CancelReload(GunChildActorReference->GetChildActor());
	}
	bIsReloading = false;
	UE_LOG(LogTemp,Warning, TEXT("Cancel Reload"));
	MainAnimInstance->Montage_Stop(0.05f, LastReloadAnimation);
	// if(GetLocalRole() < ROLE_Authority){ServerStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	// else{MulticastStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	GetOwner()->GetWorldTimerManager().ClearTimer(ReloadTimer);
}

void UShootingComponent::PickupWeapon(FWeaponData WeaponToPickup)
{
	// if(GetOwnerRole() < ROLE_Authority)
	// {
		//ServerPickupWeapon(WeaponData)
	// }
	//is the pickup a valid Weapon?
	if(WeaponToPickup.IsValid())
	{
		//Do we already have this weapon?
		//todo Change to for loop and check all weapons
		if(CurrentWeaponData.WeaponClass != WeaponToPickup.WeaponClass)
		{
			//if we already have a weapon equipped, throw the equip weapon
			if(CurrentWeaponData.WeaponClass)
			{
				//ThrowWeaponEvent(CurrentWeaponData);
				ThrowWeapon(CurrentWeaponData);
			}
			//EquipWeapon(WeaponData);
			EquipWeapon(WeaponToPickup);
		}
		//We already have this weapon
		else
		{
			//AddAmmo(WeaponData);
			AddAmmo(WeaponToPickup);
		}
	}
	else
	{
		UE_LOG(LogTemp,Error, TEXT("Invalid Weapon Data"));
	}
}

void UShootingComponent::EquipWeapon(FWeaponData WeaponToEquip)
{
	//todo not sure about < role
	// if(GetOwnerRole() < ROLE_Authority)
	// {
		// ServerEquipWeapon(WeaponData);
	// }
	AddWeaponToList(WeaponToEquip, true);
	CurrentWeaponData = WeaponToEquip;
	UE_LOG(LogTemp, Warning, TEXT("Current Ammo: %f"), WeaponToEquip.CurrentAmmo);
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
	bIsHolstered = false;
	// ClearHeldObject();	
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeapon->Execute_SetWeaponData(GunChildActorReference->GetChildActor(), CurrentWeaponData);
		//CurrentWeapon->Execute_SetWeaponAmmo(Gun->GetChildActor(), CurrentWeaponData.CurrentAmmo, CurrentWeaponData.TotalAmmoCount); // Set the gun to the players Weapon Data
		UpdateWeaponHUD();
		// PickupGunWBP(CurrentWeaponData);
	}
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
		bHasGun = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("List: %i"), CurrentWeaponListData.Num());

}

void UShootingComponent::SwapWeaponPressed()
{
	CancelReload();
	UWorld* World = GetWorld();
	if(World)
	{
		SwapWeaponActionPressedTime = World->GetTimeSeconds();
		GetOwner()->GetWorldTimerManager().SetTimer(OnWeaponSwapTimer, this, &UShootingComponent::HolsterWeapon, WeaponSwitchHoldTime, false);
	}
	else{UE_LOG(LogTemp,Error, TEXT("World Invalid on ShootingComponent %s"), *GetOwner()->GetName());}
}

void UShootingComponent::SwapWeapon()
{
	if(!bHasGun)
	{
		return;
	}
	//todo Fix all swap weapon with new weapon array
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
	
	if(PrimaryEquiped)
	{
		PrimaryEquiped = false;
		PrimaryWeaponData = CurrentWeaponData;
		CurrentWeaponData = SecondaryWeaponData;
		if(!bIsHolstered)
		{
			if(CurrentWeaponData.WeaponClass != nullptr)
			{
				EquipWeapon(CurrentWeaponData);
			}
			else
			{
				// Holstered Secondary
				UpdateWeaponHUD();
				GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
			}
		}
	}
	else
	{
		PrimaryEquiped = true;
		SecondaryWeaponData = CurrentWeaponData;
		CurrentWeaponData = PrimaryWeaponData;
		if(!bIsHolstered)
		{
			if(CurrentWeaponData.WeaponClass != nullptr)
			{
				EquipWeapon(CurrentWeaponData);
			}
			else
			{
				// Holstered Secondary
				UpdateWeaponHUD();
				GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
			}
		}
	}
	// UpdateWBP(CurrentWeaponData);
	UpdateWeaponHUD();
}

void UShootingComponent::SwapWeaponReleased()
{
	UWorld* World = GetWorld();
	if(World)
	{
		//If we have only tapped the button (Pressed time is less than hold time)
		if(World->GetTimeSeconds() - SwapWeaponActionPressedTime < WeaponSwitchHoldTime)
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
	GetOwner()->GetWorldTimerManager().ClearTimer(OnWeaponSwapTimer);
	if(bIsHolstered)
	{
		//Unholster
		bIsHolstered = false;
		if(CurrentWeaponData.WeaponClass != nullptr)
		{
			EquipWeapon(CurrentWeaponData);
		}
		else
		{
			UE_LOG(LogTemp,Warning, TEXT("InvalidWeaponData (Holster)"));
		}
	}
	else
	{
		// UpdateWBP(CurrentWeaponData);
		UpdateWeaponHUD();
		//todo Setoverlay state
		// SetOverlayState(EALSOverlayState::Default);
		bIsHolstered = true;
		GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
		// ClearHeldObject();
		// if(GetOwnerRole() < ROLE_Authority)
		// {
			// ServerClearWeapon();
		// }
		// Gun->SetChildActorClass(WeaponClassRef);
	}
}

void UShootingComponent::ThrowWeapon(FWeaponData WeaponToThrow)
{
	if(WeaponToThrow.IsValid())
	{
		CancelReload();
		DropWeapon(WeaponToThrow);
		ClearWeapon();
		GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
		//todo from owner
		//ServerDropGun(CurrentWeaponData, Location, Rotation, ThrowForce);
		// if(GetLocalRole() < ROLE_Authority)	{ServerClearWeapon();}
		// if(GetOwnerRole() < ROLE_Authority){ClearWeapon();}
		// Gun->SetChildActorClass(WeaponClassRef);
		// Gun->SetChildActorClass(EmptyWeaponData.WeaponClass);
		// WeaponDropped();
		CurrentWeaponData = EmptyWeaponData;
		// PickupGunWBP(CurrentWeaponData);
		//SetOverlayState(EALSOverlayState::Default);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon to drop"));
		return;
	}
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
	OutThrowForce = ForwardVector * (PickupThrowIntensity / Accuracy + OwnerVelocity);
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
	// Gun->SetChildActorClass(WeaponClassRef);
	GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
}

void UShootingComponent::AddAmmo(FWeaponData WeaponData)
{
	//If the total ammo is less than the max ammo && the total ammo + the pickup ammo is less than the max ammo, simply add them together
	if(CurrentWeaponData.TotalAmmoCount < CurrentWeaponData.MaxAmmo && (CurrentWeaponData.TotalAmmoCount + WeaponData.TotalAmmoCount) <= CurrentWeaponData.MaxAmmo)
	{
		CurrentWeaponData.TotalAmmoCount = CurrentWeaponData.TotalAmmoCount + WeaponData.TotalAmmoCount;
	}
	else if // If the total ammo is less than max, && the amount to pickup will end up more than the Max ammo
	(CurrentWeaponData.TotalAmmoCount < CurrentWeaponData.MaxAmmo && (CurrentWeaponData.TotalAmmoCount + WeaponData.TotalAmmoCount) > CurrentWeaponData.MaxAmmo)
	{
		CurrentWeaponData.TotalAmmoCount = CurrentWeaponData.MaxAmmo;
		//TODO::
		//Should not Destroy Actor
	}
	else // Should only cover if the ammo is Already Max 
	{
		//TODO::
		//Should not Destroy actor 
	}
	//	UpdateWBP(CurrentWeaponData);
	UpdateWeaponHUD();
}


