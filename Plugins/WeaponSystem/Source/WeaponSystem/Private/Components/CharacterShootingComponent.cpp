// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterShootingComponent.h"
#include "Interfaces/Weapon.h"
#include "Net/UnrealNetwork.h"

//- Custom log Category 
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
	PlayerWeaponState.bIsHolstered = false;
	OnWeaponEqiupped.Broadcast(WeaponInventory, CurrentWeaponIndex);
}
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
	// todo Magic blendout number 
	GetOwnerAnimInstance()->Montage_Stop(0.05f, ReloadAnimation);
	//  if(GetLocalRole() < ROLE_Authority){ServerStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	//  else{MulticastStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	// BUG CLient Doesn't work 

	OwnerActor->GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
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
		// todo
		// UpdateWeaponHUD();
		//? Testing HUD update
		OnWeaponStateChanged.Broadcast(PlayerWeaponState);
	}
}

void UCharacterShootingComponent::ShootGun()
{
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		// - Do We have ammo? 
		if(Weapon->IsClipEmpty()){return;}
		//- If we're tactical reloading, Cancel it
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
	OwnerActor->GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}

void UCharacterShootingComponent::PullTrigger()
{
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Pull Trigger"));}

	//todo Accuracy in auto is same as first bullet
	// UE_LOG(LogWeaponSystem, Warning, TEXT("Accuracy: %f"), Accuracy);
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
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
				if(PlayerCamera == nullptr){LogMissingPointer("Player Camera");return;}
				InLocation = PlayerCamera->GetCameraLocation();
				InRotation = PlayerCamera->GetCameraRotation();
			}
			Weapon->Fire(InLocation, InRotation, OwnerActor, CalculateAccuracy());
		}
		else
		{
			if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("BlindFire"));};
			Weapon->BlindFireWeapon();
			// Blindfire Animation
			//Gun needs a blindfire Function that then goes into GetTraceParams
			//Gun interface also needs function
		}
		Recoil();
		// - Update HUD elements // 
		OnAmmoChanged.Broadcast(Weapon->GetWeaponData().CurrentAmmo, Weapon->GetWeaponData().TotalAmmoCount);
		//todo Accuracy in auto is same as first bullet
	}
}

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

float UCharacterShootingComponent::CalculateAccuracy()
{
	float CalculatedAccuracy = Accuracy;
	//todo get the Stance from somewhere.. 
	// if(Stance == EALSStance::Crounching)
	{
		// CalculatedAccuracy = (CalculatedAccuracy * 1.5f);
	}
	if(OwnerActor->GetActorTimeDilation() < 1)
	{
		CalculatedAccuracy = (CalculatedAccuracy * 2.0f);	
	}
	if(!PlayerWeaponState.bIsAiming) // BlindFire
	{
		CalculatedAccuracy = (CalculatedAccuracy * 0.5f);
	}
	if(GetIsNPC())
	{
		CalculatedAccuracy = (CalculatedAccuracy * 0.25f);
	}
	return CalculatedAccuracy;
}


void UCharacterShootingComponent::PickupWeapon(FWeaponData_T WeaponToPickup,bool& bWeaponWeHave)
{
	//-  Is Weapon Valid //
	if(!WeaponToPickup.IsValid()){LogMissingPointer("Invalid Weapon data to Pickup");return;}
	//? // Run on Server
	if(GetOwnerRole() != ROLE_Authority)
	{
		ServerPickupWeapon(WeaponToPickup, bWeaponWeHave);
	}
	//- Do we have any weapons already? 
	if(!PlayerWeaponState.bHasGun && WeaponInventory.Num() == 0)
	{
		UE_LOG(LogWeaponSystem, Warning, TEXT("First Pickup"));
		AddWeaponToInventory(WeaponToPickup);
		EquipWeapon(WeaponToPickup);
		bWeaponWeHave = false;
	}
	else
	{
		//! v2
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
			bWeaponWeHave = true;
			AddAmmo(WeaponToPickup.TotalAmmoCount, Index);
		}
		else
		{
			bWeaponWeHave = false;
			CurrentWeaponIndex = WeaponInventory.Num() - 1;
			AddWeaponToInventory(WeaponToPickup);
			SwapWeapon();
		}
	}
}

void UCharacterShootingComponent::ServerPickupWeapon_Implementation(FWeaponData_T WeaponToPickup, bool bWeaponWeHave)
{
	PickupWeapon(WeaponToPickup, bWeaponWeHave);
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

		OnWeaponEqiupped.Broadcast(WeaponInventory, CurrentWeaponIndex);
	}
}

void UCharacterShootingComponent::AddAmmo(const int32 AmountToAdd, const int32 WeaponIndex)
{
	WeaponInventory[WeaponIndex].AddAmmo(AmountToAdd);
	//- If We're holding the weapon we need to add ammo to, update the guns ammo counter //
	if(CurrentWeaponIndex == WeaponIndex)
	{
		IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
		if(Weapon != nullptr)
		{
			Weapon->SetWeaponData(WeaponInventory[CurrentWeaponIndex]);
		}
	}
	
	//todo  Update WBP
}

void UCharacterShootingComponent::AddWeaponToInventory(FWeaponData_T NewWeapon)
{
	WeaponInventory.Emplace(NewWeapon);
	if(WeaponInventory.Num() > 0)
	{
		PlayerWeaponState.bHasGun = true;
	}
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Weapons: %i"), WeaponInventory.Num());}
	OnWeaponEqiupped.Broadcast(WeaponInventory, CurrentWeaponIndex);
}

void UCharacterShootingComponent::RemoveWeaponFromInventory(int32 WeaponToRemove)
{
	WeaponInventory.RemoveAt(WeaponToRemove);
	if(WeaponInventory.Num() == 0)
	{
		PlayerWeaponState.bHasGun = false;
		PlayerWeaponState.bIsHolstered = true;
	}
	OnWeaponEqiupped.Broadcast(WeaponInventory, CurrentWeaponIndex);
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Weapons: %i"), WeaponInventory.Num());}
}

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
	
	
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponInventory.Num();
	//? Should still swap if Holstered, but don't equip 
	if(PlayerWeaponState.bIsHolstered){return;}
	EquipWeapon(WeaponInventory[CurrentWeaponIndex]);
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
		OnWeaponStateChanged.Broadcast(PlayerWeaponState);

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
}

void UCharacterShootingComponent::ThrowWeapon(FWeaponData_T WeaponToThrow)
{
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Throw Weapon"));}
}

void UCharacterShootingComponent::DropWeapon()
{
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Drop Weapon"));}
	FTransform Transform;
	FVector ThrowForce;
	// GetThrowStats(Transform, ThrowForce);
	// if(WeaponToSpawn == nullptr)
	// {
	// 	UE_LOG(LogWeaponSystem,Error, TEXT("Weapon To Spawn Not Set"));
	// 	return; 
	// }
	// RemoveWeaponFromInventory(GetCurrentWeapon());
	FActorSpawnParameters SpawnParameters;
	// AActor* DroppedWeapon = GetWorld()->SpawnActor<AActor>(WeaponToSpawn, Transform.GetLocation(), Transform.GetRotation(), SpawnParameters);
}




void UCharacterShootingComponent::ServerHideWeaponModel_Implementation(bool bHidden)
{
	if(CurrentWeapon != nullptr)
	{
		CurrentWeapon->SetActorHiddenInGame(bHidden);
	}
}

// todo Test for bugs if null
void UCharacterShootingComponent::GetCurrentWeaponData(FWeaponData_T& CurrentWeaponData) const
{
	IWeapon* Weapon = Cast<IWeapon>(CurrentWeapon);
	if(Weapon != nullptr)
	{
		CurrentWeaponData = Weapon->GetWeaponData();
	}
}

////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////					Death					  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////


void UCharacterShootingComponent::Death()
{
	// if(PlayerWeaponState.bHasGun)
	// {
	// 	ThrowWeapon(GetCurrentWeapon());
	// }
	// // GunChildActorReference->SetChildActorClass(EmptyWeaponData.WeaponClass);
	// SetActive(false);
}



////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////

void UCharacterShootingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterShootingComponent, CurrentWeapon);
}
