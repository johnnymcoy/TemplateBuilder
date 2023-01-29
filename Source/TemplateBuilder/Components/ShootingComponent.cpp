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
	SetIsReplicatedByDefault(true);
}

void UShootingComponent::BeginPlay()
{
	Super::BeginPlay();
	if(!CurrentWeaponData.IsValid())
	{
		PlayerShootingStatus.bIsHolstered = true;
	}
}
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////					HUD for weapons			  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
void UShootingComponent::SetupHUD()
{
	if(bIsNPC){return;};
	if(WeaponWidgetClass == nullptr){UE_LOG(LogTemp, Error, TEXT("Add WeaponWidgetClass"))return;}
	if(OwnerPlayerController == nullptr){UE_LOG(LogTemp, Error, TEXT("Player Controller Failed")) return;}
	WeaponWidget = CreateWidget<UWeaponWidget>(OwnerPlayerController, WeaponWidgetClass);
	if(WeaponWidget == nullptr){UE_LOG(LogTemp, Error, TEXT("WeaponWidget FAILED"))return;}
	WeaponWidget->AddToViewport();
}

//Update Ammo with a slight delay so there's time for the gun to fire
void UShootingComponent::UpdateWeaponHUD()
{
	GetOwner()->GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &UShootingComponent::DelayUpdateWeaponHUD, 0.001f, false);
}
void UShootingComponent::DelayUpdateWeaponHUD()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeaponData = CurrentWeapon->GetWeaponData();
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
		WeaponWidget->SetWeaponData(PrimaryWeaponData, SecondaryWeaponData, PlayerShootingStatus.bPrimaryEquipped, PlayerShootingStatus.bSecondaryEquipped, PlayerShootingStatus.bIsHolstered);
	}
	//Add crosshair functions
}
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////					Ammo UMG				  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
void UShootingComponent::AimPressed(bool bRightShoulder)
{
	PlayerShootingStatus.bIsAiming = true;
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon && CurrentWeaponData.IsValid())
	{
		CurrentWeapon->MoveUMG(!bRightShoulder);
		CurrentWeapon->FadeInUMG(PlayerShootingStatus.bIsAiming);
		UE_LOG(LogTemp, Warning, TEXT("Fade UMG - ShootingComponent"));
	}
}
void UShootingComponent::AimReleased(bool bRightShoulder)
{
	PlayerShootingStatus.bIsAiming = false;
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon && CurrentWeaponData.IsValid())
	{
		CurrentWeapon->MoveUMG(!bRightShoulder);
		CurrentWeapon->FadeInUMG(PlayerShootingStatus.bIsAiming);
	}
}
void UShootingComponent::MoveUMG(bool bRightShoulder)
{
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon && CurrentWeaponData.IsValid())
	{
		CurrentWeapon->MoveUMG(bRightShoulder);
	}
}
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////					Shooting				  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
void UShootingComponent::ValidateShootGun()
{
	if(CurrentWeaponData.CurrentAmmo <= 0){return;};
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		if(PlayerShootingStatus.bIsReloading && CurrentWeaponData.CurrentAmmo > 0){CancelReload();}
		if(CurrentWeapon->IsInAutoMode())
		{
			//todo remove firerate / 200.0f
			GetOwner()->GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &UShootingComponent::ShootGun, (CurrentWeaponData.FireRate/ 200.f), true, 0.01f);
		}
		else
		{
			ShootGun();
		}
	}
}

void UShootingComponent::ShootGun()
{
	if(CurrentWeaponData.CurrentAmmo <= 0){return;};
	//todo Accuracy in auto is same as first bullet
	// UE_LOG(LogTemp, Warning, TEXT("Accuracy: %f"), Accuracy);
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
			CurrentWeapon->GetTraceParams(InLocation, InRotation, GetOwner(), Accuracy);
		}
		else
		{
			CurrentWeapon->BlindFireWeapon();
			UE_LOG(LogTemp, Warning, TEXT("BlindFire"));
			//BlindFire Animation
			//Gun needs a blindfire Function that then goes into GetTraceParams
			//Gun interface also needs function
		}
		Recoil();
		UpdateWeaponHUD();
		//todo Accuracy in auto is same as first bullet
	}
}

void UShootingComponent::StopShootGun()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}

void UShootingComponent::Recoil()
{
	RecoilAmount = CurrentWeaponData.Recoil;
	float RecoilTotal;
	//TODO Get rid of Magic number (4 and 8 multiplyer)
	if(PlayerShootingStatus.bIsAiming){RecoilTotal = (FMath::RandRange(RecoilAmount, (RecoilAmount * 4)) / Accuracy);}
	else{RecoilTotal = (FMath::RandRange(RecoilAmount, (RecoilAmount * 8)) / Accuracy);}
	OnBulletShot.Broadcast(RecoilTotal);
}

void UShootingComponent::SwitchAutoMode()
{
	if(!CurrentWeaponData.bHasAutoMode){return;}
	StopShootGun();
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(GunChildActorReference->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeapon->SwitchAutoMode();
		UpdateWeaponHUD();
	}
}
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////					Reload					  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
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
				CurrentWeapon->Reload(ReloadTime);
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
			CurrentWeaponData = CurrentWeapon->GetWeaponData();
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
		CurrentWeapon->CancelReload();
	}
	PlayerShootingStatus.bIsReloading = false;
	MainAnimInstance->Montage_Stop(0.05f, LastReloadAnimation);
	// if(GetLocalRole() < ROLE_Authority){ServerStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	// else{MulticastStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	//BUG CLient Doesn't work 
	GetOwner()->GetWorldTimerManager().ClearTimer(ReloadTimer);
}
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////					Pickup					  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
void UShootingComponent::PickupWeapon(FWeaponData WeaponToPickup)
{
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
	if(!WeaponToPickup.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Pickup Weapon Data"));
		return;
	}
	//todo if we have full ammo
	if(GetOwnerRole() != ROLE_Authority)
	{
		ServerPickupWeapon(WeaponToPickup);
	}
	else
	{
		//If we don't have any weapon
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
			UE_LOG(LogTemp,Warning, TEXT("Pickup Primary ammo"));
			return;
		}
		///Check if we already have it as Secondary 
		if(SecondaryWeaponData.WeaponClass == WeaponToPickup.WeaponClass)
		{
			AddAmmo(WeaponToPickup, false);
			UE_LOG(LogTemp,Warning, TEXT("Pickup Secondary ammo"));
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
	// bHasGun = true;
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
		CurrentWeapon->SetWeaponData(CurrentWeaponData);
		OnStateChange.Broadcast(CurrentWeaponData.OverlayState);
		UpdateWeaponHUD();
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
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////				Multiple Weapons WIP		  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
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
		// bHasGun = true;
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
	if(!PlayerShootingStatus.bHasGun){return;}
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
		OnStateChange.Broadcast(EALSOverlayState::Default);
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
			EquipWeapon(CurrentWeaponData, PlayerShootingStatus.bPrimaryEquipped);
		}
		else{UE_LOG(LogTemp,Warning, TEXT("InvalidWeaponData (Holster)"));}
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
	CancelReload();
	DropWeapon(WeaponToThrow);
	ClearWeapon();
	GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
	CurrentWeaponData = EmptyWeaponData;
	UpdateWeaponHUD();
	OnStateChange.Broadcast(EALSOverlayState::Default);
	//todo from owner
	//ServerDropGun(CurrentWeaponData, Location, Rotation, ThrowForce);
	// if(GetLocalRole() < ROLE_Authority)	{ServerClearWeapon();}
	// if(GetOwnerRole() < ROLE_Authority){ClearWeapon();}
	// WeaponDropped();
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
		OnStateChange.Broadcast(EALSOverlayState::Default);
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


////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////					Pickup Ammo				  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////


//v2.0
//go through Full weapon list
//for(WeaponsList)
// if Weapon picking up is == to a weapon in the list
// Add ammos together 


void UShootingComponent::AddAmmo(const FWeaponData in_WeaponData, const bool in_bIsPrimary)
{
	FWeaponData TempWeaponData;
	if(in_bIsPrimary)
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
	if(TempWeaponData.TotalAmmoCount < TempWeaponData.MaxAmmo && (TempWeaponData.TotalAmmoCount + in_WeaponData.TotalAmmoCount) <= TempWeaponData.MaxAmmo)
	{
		TempWeaponData.TotalAmmoCount = TempWeaponData.TotalAmmoCount + in_WeaponData.TotalAmmoCount;
	}
	else if // If the total ammo is less than max, && the amount to pickup will end up more than the Max ammo
	(TempWeaponData.TotalAmmoCount < TempWeaponData.MaxAmmo && (TempWeaponData.TotalAmmoCount + in_WeaponData.TotalAmmoCount) > TempWeaponData.MaxAmmo)
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
	
	if(in_bIsPrimary)
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
		CurrentWeapon->SetWeaponData(CurrentWeaponData);
		UpdateWeaponHUD();
		// PickupGunWBP(CurrentWeaponData);
	}
}
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////					Death					  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
void UShootingComponent::Death()
{
	if(PlayerShootingStatus.bHasGun)
	{
		ThrowWeapon(CurrentWeaponData);
	}
	GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
	SetActive(false);
}

void UShootingComponent::SetController(AController* Controller)
{
	OwnerController = Controller;
	if(bIsNPC)
	{
		OwnerAIController = Cast<AAIController>(Controller);
	}
	else
	{
		OwnerPlayerController = Cast<APlayerController>(Controller);
	}
}

void UShootingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UShootingComponent, GunChildActorReference);
	DOREPLIFETIME(UShootingComponent, CurrentWeaponData);
	DOREPLIFETIME(UShootingComponent, PlayerShootingStatus);
}
