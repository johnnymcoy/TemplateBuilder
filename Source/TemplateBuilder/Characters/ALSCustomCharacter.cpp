// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSCustomCharacter.h"
#include "TemplateBuilder/Weapons/WeaponInterface.h"
#include "Character/Animation/ALSCharacterAnimInstance.h"
#include "TemplateBuilder/Weapons/WeaponBase.h"
#include "TemplateBuilder/Interactable/WeaponPickupBase.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TemplateBuilder/Health/HealthComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TemplateBuilder/Interactable/InteractableInterface.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "ALSCustomController.h"
#include "Net/UnrealNetwork.h"

//TODO:
// Picking up gun always destroys actor

//Constructor
AALSCustomCharacter::AALSCustomCharacter(const FObjectInitializer& ObjectInitializer)
: AALSCharacter (ObjectInitializer)
{
	Gun = CreateDefaultSubobject<UChildActorComponent>(TEXT("GUN"));
	Gun->SetupAttachment(HeldObjectRoot);
	Gun->SetChildActorClass(AWeaponBase::StaticClass());
	Gun->SetIsReplicated(true);
	
	ThrowPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ThrowPoint"));
	ThrowPoint->SetupAttachment(GetMesh(), TEXT("head"));
	
	PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimation"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &AALSCustomCharacter::OnHealthChanged);
	OnTakeAnyDamage.AddDynamic(this, &AALSCustomCharacter::AnyDamageTaken);
	SetupPhysicalAnimationDefaults();
	//Defaults incase nothing is set
	PickupThrowIntensity = 500;
}

void AALSCustomCharacter::BeginPlay() 
{
	Super::BeginPlay();
	DefaultAccuracy = Accuracy;
	SetupPhysicalAnimation();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AALSCustomCharacter::OnOverlapBegin);
	if(CurrentWeaponData.MeshForPickup == nullptr){bIsHolstered = true;}
	SkeletalMesh->SetHiddenInGame(true);
}

void AALSCustomCharacter::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
}

void AALSCustomCharacter::SetupPhysicalAnimation() 
{
	PhysicalAnimation->SetSkeletalMeshComponent(GetMesh());
	PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow(FName("Pelvis"), PelvisData, true);
	PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow(FName("spine_01"), SpineData, true);
	PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow(FName("Thigh_L"), LegData, true);
	PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow(FName("Thigh_R"), LegData, true);
	PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow(FName("clavicle_l"), ArmData, true);
	PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow(FName("clavicle_r"), ArmData, true);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("Pelvis"), true, true);
}

void AALSCustomCharacter::SetupPhysicalAnimationDefaults() 
{
	PelvisData.bIsLocalSimulation = false;
	SpineData.bIsLocalSimulation = false;
	LegData.bIsLocalSimulation = false;
	ArmData.bIsLocalSimulation = false;
	PelvisData.OrientationStrength = 10000.0f;
	SpineData.OrientationStrength = 10000.0f;
	LegData.OrientationStrength = 10000.0f;
	ArmData.OrientationStrength = 10000.0f;
	PelvisData.AngularVelocityStrength = 300.0f;
	SpineData.AngularVelocityStrength = 500.0f;
	LegData.AngularVelocityStrength = 500.0f;
	ArmData.AngularVelocityStrength = 500.0f;
	PelvisData.PositionStrength = 2000.0f;
	SpineData.PositionStrength = 1000.0f;
	LegData.PositionStrength = 1000.0f;
	ArmData.PositionStrength = 500.0f;
	PelvisData.VelocityStrength = 100.0f;
	PelvisData.MaxLinearForce = 3000.0f;
	SpineData.MaxLinearForce = 50000.0f;
	LegData.MaxLinearForce = 50000.0f;
	ArmData.MaxLinearForce = 50000.0f;
	PelvisData.MaxAngularForce = 3000.0f;
	SpineData.MaxAngularForce = 50000.0f;
	LegData.MaxAngularForce = 100000.0f;
	ArmData.MaxAngularForce = 3000.0f;
}

void AALSCustomCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) 
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("ShootAction", IE_Pressed, this, &AALSCustomCharacter::ShootGunCheck);
	PlayerInputComponent->BindAction("ShootAction", IE_Released, this, &AALSCustomCharacter::StopShootGun);
	PlayerInputComponent->BindAction("AimAction", IE_Pressed, this, &AALSCustomCharacter::AimPressedAction);
	PlayerInputComponent->BindAction("AimAction", IE_Released, this, &AALSCustomCharacter::AimReleasedAction);
	PlayerInputComponent->BindAction("ReloadAction", IE_Pressed, this, &AALSCustomCharacter::Reload);
	PlayerInputComponent->BindAction("AutoModeAction", IE_Pressed, this, &AALSCustomCharacter::SwitchAutoMode);
	PlayerInputComponent->BindAction("SwapWeaponAction", IE_Pressed, this, &AALSCustomCharacter::SwapWeaponPressed);
	PlayerInputComponent->BindAction("SwapWeaponAction", IE_Released, this, &AALSCustomCharacter::SwapWeaponReleased);
	PlayerInputComponent->BindAction("CameraAction", IE_Pressed, this, &AALSCustomCharacter::CameraButtonPressed);
}

void AALSCustomCharacter::AimPressedAction() 
{
	Super::AimPressedAction();
	bIsAiming = true;
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(Gun->GetChildActor());
	if(CurrentWeapon && CurrentWeaponData.MeshForPickup)
	{
		// CurrentWeapon->FadeUMG();
		UE_LOG(LogTemp,Warning,TEXT("Fade UMG"));
	}
}

void AALSCustomCharacter::AimReleasedAction() 
{
	Super::AimReleasedAction();
	bIsAiming = false;
}

void AALSCustomCharacter::CameraButtonPressed() 
{
	CameraPressedAction();
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(Gun->GetChildActor());
	if(CurrentWeapon && CurrentWeaponData.MeshForPickup)
	{
		UE_LOG(LogTemp,Warning,TEXT("Move UMG Side"));
	}
}

void AALSCustomCharacter::ShootGun() //Calculated every bullet
{
	// If we are holding a Gun
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(Gun->GetChildActor());
	if(CurrentWeapon)
	{
		FVector InLocation;
		FRotator InRotation;
		if(!bIsNPC) //If Player get the Camera
		{
			APlayerCameraManager* PlayerCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
			InLocation = PlayerCamera->GetCameraLocation();
			InRotation = PlayerCamera->GetCameraRotation();
		}
		else //If NPC get ViewPoint 
		{
			GetActorEyesViewPoint(InLocation, InRotation);
		}
		CalculateAccuracy();
		//Need to change name so it's more fittting From: GetTraceParams -> TO :ShootGun
		if(bIsAiming)
		{
			CurrentWeapon->Execute_GetTraceParams(Gun->GetChildActor(), InLocation, InRotation, this, Accuracy);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BlindFire"));
			//BlindFire Animation
			//Gun needs a blindfire Function that then goes into GetTraceParams
			//Gun interface also needs function
		}
		//Update Ammo with a slight delay so theirs time for the gun to fire
		GetWorldTimerManager().SetTimer(HudUpdateHandle, this, &AALSCustomCharacter::UpdateWBPDelayed, 0.01f, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Please Add a Gun Child Class in editor"));
	}
}

void AALSCustomCharacter::UpdateWBPDelayed() 
{
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(Gun->GetChildActor());
    if (!ensure(CurrentWeapon != nullptr)) return;
	CurrentWeaponData = CurrentWeapon->Execute_GetWeaponData(Gun->GetChildActor());
	if(CurrentWeaponData.CurrentAmmo > 0)
	{
		Recoil();
	}
	if(!bIsNPC) UpdateWBP(CurrentWeaponData);

}

void AALSCustomCharacter::ShootGunCheck() 
{
	if(!bIsHolstered)
	{
		if(MantleTimeline->IsPlaying()){return;}
		IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(Gun->GetChildActor());
		if(CurrentWeapon)
		{
			if(bIsReloading && CurrentWeaponData.CurrentAmmo > 0){CancelReload();}
			if(CurrentWeapon->Execute_IsInAutoMode(Gun->GetChildActor()))
			{
				//Not good to have magic number (/200.f)
				GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &AALSCustomCharacter::ShootGun, (CurrentWeaponData.FireRate/ 200.f), true, 0.01f);
			}
			else
			{
				ShootGun();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Melee Mode?"));
		//MeleeFunction();
		//Setoverlay State to melee;
		//punch
		//Linetrace the punch 
		//add force to hit
		//set timer so can't double press, or check before the animation finishes 
		// Could have it held down continues to hit? like auto mode on a gun? although why would you not just hold it then.. 
	}
}

void AALSCustomCharacter::StopShootGun() 
{
	GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}

void AALSCustomCharacter::Recoil() 
{
	RecoilAmount = CurrentWeaponData.Recoil;
	float RecoilTotal;
	if(bIsAiming){RecoilTotal = (FMath::RandRange(RecoilAmount, (RecoilAmount * 4)) / Accuracy);}
	else{RecoilTotal = (FMath::RandRange(RecoilAmount, (RecoilAmount * 8)) / Accuracy);} //Recoil Multiplier Magic number needs to be changed
	AddControllerPitchInput(RecoilTotal);
	AddRecoilWBP(RecoilTotal);
}

void AALSCustomCharacter::CalculateAccuracy() 
{
	Accuracy = DefaultAccuracy; //Resets each time
	if(Stance == EALSStance::Crouching) // If crouching add accuracy// possibly get new function
	{
		Accuracy = (Accuracy * 1.5f);
	}
	if(GetActorTimeDilation() < 1)
	{
		Accuracy = (Accuracy * 2.0f); 
	}
	if(!bIsAiming) // BlindFire 
	{
		Accuracy = (Accuracy * 0.5f);
	}
}

void AALSCustomCharacter::SwitchAutoMode() 
{
	StopShootGun();
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(Gun->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeapon->Execute_SwitchAutoMode(Gun->GetChildActor());
	}
}

void AALSCustomCharacter::RagdollStart() 
{
	Super::RagdollStart();
	PhysicalAnimation->SetStrengthMultiplyer(0);
}

void AALSCustomCharacter::RagdollEnd() 
{
	Super::RagdollEnd();
	PhysicalAnimation->SetStrengthMultiplyer(1);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("Pelvis"), true, true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetWorldTimerManager().SetTimer(RagdollGroundTimer, this, &AALSCustomCharacter::RagdollMeshFix, 1.0f, false);
	ClearHeldObject();
}

void AALSCustomCharacter::RagdollMeshFix() 
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetWorldTimerManager().ClearTimer(RagdollGroundTimer);
}

void AALSCustomCharacter::Death_Implementation() 
{
	ReplicatedRagdollStart();
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerClearWeapon();
	}
	Gun->SetChildActorClass(WeaponClassRef);
	SetReplicatingMovement(true);
	FVector Location;
	FRotator Rotation;
	FVector Scale;
	FVector ThrowForce;
	GetThrowStats(Location, Rotation, Scale, ThrowForce);
	ServerDropGun(CurrentWeaponData, Location, Rotation, ThrowForce);
	GetMovementComponent()->StopMovementImmediately();
	//DetachFromControllerPendingDestroy(); 
	//JUST Player? AI buggy?
	//TEMP:
	AALSCustomController* PlayerControllerRef = Cast<AALSCustomController>(GetController());
	if(PlayerControllerRef)
	{
		PlayerControllerRef->Disable();
	}
	//Set input 
} 

void AALSCustomCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	IInteractableInterface* Interface = Cast<IInteractableInterface>(OtherActor);
	if(Interface)
	{
		Interface->Execute_OnPickUp(OtherActor, this);
	}
}

void AALSCustomCharacter::OnHealthChanged(UHealthComponent* HealthComp, float Health, float DefaultHealth, float Shield, float DefaultShield, const class UDamageType* DamageType) 
{
	if(Health <= 0.0f && !DeathOnce)
	{
		if(!DeathOnce) //only plays once
		{
			DeathOnce = true;
			Death();
			UE_LOG(LogTemp, Warning, TEXT("%s is DEAD"), *GetName());
		}
	}
}

void AALSCustomCharacter::Reload() 
{	
	if(!bIsReloading)
	{
		IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(Gun->GetChildActor());
		if(CurrentWeapon)
		{	
			if(CurrentWeaponData.CurrentAmmo < CurrentWeaponData.ClipSize && CurrentWeaponData.TotalAmmoCount > 0)
			{
				float ReloadTime = MainAnimInstance->Montage_Play(GetReloadAnimation(CurrentWeaponData.WeaponType), (1.0f / CurrentWeaponData.ReloadTime), EMontagePlayReturnType::Duration, 0.0f, true);
				if(GetLocalRole() < ROLE_Authority){ServerPlayMontageAnimation(GetReloadAnimation(CurrentWeaponData.WeaponType), (1.0f / CurrentWeaponData.ReloadTime), EMontagePlayReturnType::Duration, 0.0f, true);}
				else{MulticastPlayMontageAnimation(GetReloadAnimation(CurrentWeaponData.WeaponType), (1.0f / CurrentWeaponData.ReloadTime), EMontagePlayReturnType::Duration, 0.0f, true);}
				CurrentWeapon->Execute_Reload(Gun->GetChildActor(), ReloadTime);
				bIsReloading = true;
				GetWorldTimerManager().SetTimer(ReloadTimer, this, &AALSCustomCharacter::ReloadDelay, (ReloadTime + 0.1f), false); //Offset so that the HUD isn't offset
			}
		}
	}
}

void AALSCustomCharacter::ServerPlayMontageAnimation_Implementation(UAnimMontage* MontageToPlay, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages) 
{
	MainAnimInstance->Montage_Play(MontageToPlay, InPlayRate, ReturnValueType, InTimeToStartMontageAt, bStopAllMontages);
	MulticastPlayMontageAnimation(MontageToPlay, InPlayRate, ReturnValueType, InTimeToStartMontageAt, bStopAllMontages);
}

void AALSCustomCharacter::MulticastPlayMontageAnimation_Implementation(UAnimMontage* MontageToPlay, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages) 
{
	MainAnimInstance->Montage_Play(MontageToPlay, InPlayRate, ReturnValueType, InTimeToStartMontageAt, bStopAllMontages);
}

void AALSCustomCharacter::ServerStopMontageAnimation_Implementation(float InBlendOutTime, const UAnimMontage* Montage) 
{
	MainAnimInstance->Montage_Stop(InBlendOutTime, Montage);
	MulticastStopMontageAnimation(InBlendOutTime, Montage);
}

void AALSCustomCharacter::MulticastStopMontageAnimation_Implementation(float InBlendOutTime, const UAnimMontage* Montage) 
{
	MainAnimInstance->Montage_Stop(InBlendOutTime, Montage);
}

void AALSCustomCharacter::ReloadDelay() 
{
	GetWorldTimerManager().ClearTimer(ReloadTimer);
	if(bIsReloading)
	{
		IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(Gun->GetChildActor());
		if(CurrentWeapon)
		{	
			CurrentWeaponData = CurrentWeapon->Execute_GetWeaponData(Gun->GetChildActor());
			// CurrentWeaponData.CurrentAmmo = CurrentWeapon->Execute_GetCurrentAmmo(Gun->GetChildActor());
			// CurrentWeaponData.TotalAmmoCount = CurrentWeapon->Execute_GetTotalAmmoCount(Gun->GetChildActor());
			if(!bIsNPC){UpdateWBP(CurrentWeaponData);}
		}
		bIsReloading = false;
	}
}

void AALSCustomCharacter::CancelReload() 
{
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(Gun->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeapon->Execute_CancelReload(Gun->GetChildActor());
	}
	bIsReloading = false;
	UE_LOG(LogTemp, Warning, TEXT("Cancel Reload"));
	MainAnimInstance->Montage_Stop(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));
	if(GetLocalRole() < ROLE_Authority){ServerStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	else{MulticastStopMontageAnimation(0.05f, GetReloadAnimation(CurrentWeaponData.WeaponType));}
	GetWorldTimerManager().ClearTimer(ReloadTimer);
}

void AALSCustomCharacter::AnyDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) 
{
	GetWorldTimerManager().SetTimer(HealthTimer, this, &AALSCustomCharacter::HealthDelay, 0.001f, false);
}

void AALSCustomCharacter::HealthDelay() 
{
	UpdateHealthWBP(HealthComponent->Health, HealthComponent->DefaultHealth, HealthComponent->ShieldHealth, HealthComponent->DefaultShieldHealth);
}

void AALSCustomCharacter::UpdateHealthWBP_Implementation(float Health, float DefaultHealth, float Shield, float DefaultShield) 
{
	GetWorldTimerManager().ClearTimer(HealthTimer);
}

void AALSCustomCharacter::AddRecoilWBP_Implementation(float RecoilAmmount) 
{
	//Default
}

/// Interface Calls
void AALSCustomCharacter::AddImpulseEvent_Implementation(const FVector in_Impulse, const FName in_HitBone, const float in_GunImpulse) 
{
	FVector BulletForce;
	BulletForce.X = (in_Impulse.X * UKismetMathLibrary::RandomFloatInRange(0.3f, in_GunImpulse));
	BulletForce.Y = (in_Impulse.Y * UKismetMathLibrary::RandomFloatInRange(0.3f, in_GunImpulse));
	BulletForce.Z = (in_Impulse.Z * UKismetMathLibrary::RandomFloatInRange(0.3f, in_GunImpulse));
	FVector Location;
	GetMesh()->AddImpulseAtLocation(BulletForce, Location, in_HitBone);
	GetMesh()->AddImpulseToAllBodiesBelow((BulletForce / 2), in_HitBone, true, true);
	if(HealthComponent->GetHealth() == 0)
	{
		if(!DoOnce)
		{
			GetMesh()->AddImpulseToAllBodiesBelow((BulletForce), in_HitBone, true, true);
			DoOnce = true;
		}
	}
}

void AALSCustomCharacter::BulletDamageEvent_Implementation(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone,
 											AController* in_EventInstigator, AActor* in_DamageCauser,  TSubclassOf<class UDamageType> in_DamageType) 
{
	// UE_LOG(LogTemp, Warning, TEXT("damage type; %s"), *in_DamageType->GetName());
	HealthComponent->LimbDamage(in_Damage, in_HeadMultiplier, in_HitBone, in_DamageType);
	if(in_HitBone == FName("head"))
	{
		float HeadshotDamage = (in_Damage * in_HeadMultiplier);
		UGameplayStatics::ApplyDamage(this, HeadshotDamage, in_EventInstigator, in_DamageCauser, in_DamageType);
	}
	else
	{
		UGameplayStatics::ApplyDamage(this, in_Damage, in_EventInstigator, in_DamageCauser, in_DamageType);
	}
	// if(HealthComponent->Health < in_Damage)
	// {
	// 	if(!DeathOnce) //only plays once
	// 	{
	// 		DeathOnce = true;
	// 		Death();
	// 	}
	// }
	if(HealthComponent->Health <= (HealthComponent->DefaultHealth / 3) && IsDead() == false) // third health left
	{
		UE_LOG(LogTemp, Warning, TEXT("InjuredState"));
	}
}

void AALSCustomCharacter::PickupGunEvent_Implementation(const FWeaponData in_WeaponData) 
{
	PickupWeapon(in_WeaponData);
}

void AALSCustomCharacter::UpdateWBP_Implementation(FWeaponData WeaponData) 
{
	//Default UpdateWBP Stuff
}


void AALSCustomCharacter::PickupGunWBP_Implementation(FWeaponData WeaponData) 
{
	//Default
}

void AALSCustomCharacter::PickupWeapon(FWeaponData WeaponData) 
{
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerPickupWeapon(WeaponData);
	}
	if(WeaponData.WeaponClass != nullptr) //Weapon is valid check
	{
		if(CurrentWeaponData.WeaponClass != WeaponData.WeaponClass)// && PrimaryWeaponData.WeaponClass != WeaponData.WeaponClass && SecondaryWeaponData.WeaponClass != WeaponData.WeaponClass)	//Don't have this weapon
		{
			if(CurrentWeaponData.WeaponClass)
			{
				ThrowWeaponEvent(CurrentWeaponData);
			}
			EquipWeapon(WeaponData);
		}
		else	//We already have this gun
		{
			AddAmmo(WeaponData);
		}	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Weapon Data"));
	}
}


void AALSCustomCharacter::ServerPickupWeapon_Implementation(FWeaponData WeaponData) 
{
	PickupWeapon(WeaponData);
}

bool AALSCustomCharacter::ServerPickupWeapon_Validate(FWeaponData WeaponData) 
{
	return true;
}

void AALSCustomCharacter::AddAmmo(FWeaponData WeaponData) 
{
	//If the total ammo is less than the max ammo && the total ammo + the pickup ammo is less than the max ammo, simply add them together
	if(CurrentWeaponData.TotalAmmoCount < CurrentWeaponData.MaxAmmo && (CurrentWeaponData.TotalAmmoCount + WeaponData.TotalAmmoCount) <= CurrentWeaponData.MaxAmmo)
	{
		CurrentWeaponData.TotalAmmoCount = CurrentWeaponData.TotalAmmoCount + WeaponData.TotalAmmoCount;
	}
	else if // If the total ammo is less than max, && the ammount to pickup will end up more than the Max ammo
	(CurrentWeaponData.TotalAmmoCount < CurrentWeaponData.MaxAmmo && (CurrentWeaponData.TotalAmmoCount + WeaponData.TotalAmmoCount) > CurrentWeaponData.MaxAmmo)
	{
		CurrentWeaponData.TotalAmmoCount = CurrentWeaponData.MaxAmmo;
		//TODO::
		//Should not Destroy Actor
	}
	else // Should only cover if the ammo is Already Max 
	{
		//TODO::
		//Should not Destory actor 
	}
	UpdateWBP(CurrentWeaponData);
}

// timer function Swap weapon
void AALSCustomCharacter::SwapWeaponPressed()
{
	CancelReload();
	UWorld* World = GetWorld();
	check(World);
	SwapWeaponActionPressedTime = World->GetTimeSeconds();
	GetWorldTimerManager().SetTimer(OnWeaponSwapTimer, this, &AALSCustomCharacter::HolsterWeapon, WeaponSwitchHoldTime, false);
}

void AALSCustomCharacter::SwapWeaponReleased()
{
	UWorld* World = GetWorld();
	check(World);
	//If we have only tapped the button (Pressed time is less than hold time)
	if (World->GetTimeSeconds() - SwapWeaponActionPressedTime < WeaponSwitchHoldTime)
	{
		SwapWeapon();
		GetWorldTimerManager().ClearTimer(OnWeaponSwapTimer); //Prevent Holster Weapon
	}
}

void AALSCustomCharacter::SwapWeapon() 
{
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
				NoSecondaryEquip(); // Holstered Secondary
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
				NoSecondaryEquip(); // Holstered Secondary
			}
		}
	}
	UpdateWBP(CurrentWeaponData);
}

//Replicated// 

void AALSCustomCharacter::HolsterWeapon() 
{
	GetWorldTimerManager().ClearTimer(OnWeaponSwapTimer);
	if(bIsHolstered)
	{
		bIsHolstered = false;
		if(CurrentWeaponData.WeaponClass != nullptr)
		{
			EquipWeapon(CurrentWeaponData);
		}
		else
		{
			UE_LOG(LogTemp,Warning, TEXT("InvalidWeaponDataHolster"));
			return;
		}
		return;
	}
	else
	{
		UpdateWBP(CurrentWeaponData);
		SetOverlayState(EALSOverlayState::Default);
		bIsHolstered = true;
		ClearHeldObject();
		if(GetLocalRole() < ROLE_Authority)
		{
			ServerClearWeapon();
		}
		Gun->SetChildActorClass(WeaponClassRef);
	}
}

void AALSCustomCharacter::NoSecondaryEquip() 
{
	UpdateWBP(CurrentWeaponData);
	SetOverlayState(EALSOverlayState::Default);
	ClearHeldObject();
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerClearWeapon();
	}
	Gun->SetChildActorClass(WeaponClassRef);
}

void AALSCustomCharacter::ServerClearWeapon_Implementation() 
{
	Gun->SetChildActorClass(WeaponClassRef);
}

void AALSCustomCharacter::EquipWeapon(FWeaponData WeaponData) 
{
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerEquipWeapon(WeaponData);
	}
	CurrentWeaponData = WeaponData;
	int32 CurrentAmmoTest = CurrentWeaponData.CurrentAmmo;
	// *FString::SanitizeFloat(OverShieldAmount), *GetOwner()->GetName());
	UE_LOG(LogTemp,Warning,TEXT("(Player)Current Ammo = %s"), *FString::FromInt(CurrentAmmoTest));
	Gun->SetChildActorClass(CurrentWeaponData.WeaponClass);
	SetOverlayState(CurrentWeaponData.OverlayState);
	FName SocketName;
	if(CurrentWeaponData.bIsLeftHand)
	{SocketName = TEXT("VB LHS_ik_hand_gun");}
	else
	{SocketName = TEXT("VB RHS_ik_hand_gun");}
	Gun->GetChildActor()->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepWorldTransform, SocketName);
	Gun->GetChildActor()->GetRootComponent()->SetRelativeLocation(CurrentWeaponData.WeaponOffset);
	Gun->GetChildActor()->GetRootComponent()->SetRelativeRotation(CurrentWeaponData.RotationOffset);
	bIsHolstered = false;
	Gun->GetChildActor()->SetOwner(this);
	ClearHeldObject();	
	IWeaponInterface* CurrentWeapon = Cast<IWeaponInterface>(Gun->GetChildActor());
	if(CurrentWeapon)
	{
		CurrentWeapon->Execute_SetWeaponData(Gun->GetChildActor(), CurrentWeaponData);
		//CurrentWeapon->Execute_SetWeaponAmmo(Gun->GetChildActor(), CurrentWeaponData.CurrentAmmo, CurrentWeaponData.TotalAmmoCount); // Set the gun to the players Weapon Data
		PickupGunWBP(CurrentWeaponData);
	}
}

void AALSCustomCharacter::ServerEquipWeapon_Implementation(FWeaponData WeaponData) 
{
	EquipWeapon(WeaponData);
}

void AALSCustomCharacter::OnRep_ClearWeapon() 
{
	ClearHeldObject(); // For Removing the Standard ALS pistol etc.
}

bool AALSCustomCharacter::IsDead() const
{
	if(HealthComponent->Health > 0) 
	{
		return false;
	}
	return true;
}

void AALSCustomCharacter::GetThrowStats(FVector &OutLocation, FRotator &OutRotation, FVector &OutScale, FVector &OutThrowForce) const
{
	OutLocation = ThrowPoint->GetComponentLocation();
	OutRotation.Yaw = UKismetMathLibrary::RandomFloatInRange(-180.0f, 180.0f);
	OutRotation.Roll = UKismetMathLibrary::RandomFloatInRange(-180.0f, 180.0f);
	OutRotation.Pitch = UKismetMathLibrary::RandomFloatInRange(-180.0f, 180.0f);
	OutScale = ThrowPoint->GetComponentScale();
	FRotator Rotation = ThrowPoint->GetComponentRotation();
	FVector ForwardVector = (FRotationMatrix(Rotation).GetScaledAxis( EAxis::X ));
	OutThrowForce = ForwardVector * PickupThrowIntensity;
}

void AALSCustomCharacter::ThrowWeaponEvent_Implementation(FWeaponData WeaponData) 
{
	if(WeaponData.MeshForPickup == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon to drop"));
		return;
	}
	else
	{
		CancelReload();
		FVector Location;
		FRotator Rotation;
		FVector Scale;
		FVector ThrowForce;
		GetThrowStats(Location, Rotation, Scale, ThrowForce);
		ServerDropGun(CurrentWeaponData, Location, Rotation, ThrowForce);
		if(GetLocalRole() < ROLE_Authority)	{ServerClearWeapon();}
		Gun->SetChildActorClass(WeaponClassRef);
		WeaponDropped();
	}
}

void AALSCustomCharacter::ServerDropGun_Implementation(FWeaponData SpawnWeaponData, FVector Location, FRotator Rotation, FVector ThrowForce) 
{
	if(GunToSpawn != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		AWeaponPickupBase* DroppedWeaponRef = GetWorld()->SpawnActor<AWeaponPickupBase>(GunToSpawn, Location, Rotation, SpawnParams);
		DroppedWeaponRef->WeaponDroppedEvent(SpawnWeaponData); //Set the pickups weapon stats 
		DroppedWeaponRef->GunMesh->AddImpulse(ThrowForce, NAME_None, true); // Throw the weapon
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GunToSpawn not set"));
	}
	MulticastDropGun(SpawnWeaponData, Location, Rotation, ThrowForce);
}

void AALSCustomCharacter::MulticastDropGun_Implementation(FWeaponData SpawnWeaponData, FVector Location, FRotator Rotation, FVector ThrowForce) 
{
	if(GunToSpawn != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		AWeaponPickupBase* DroppedWeaponRef = GetWorld()->SpawnActor<AWeaponPickupBase>(GunToSpawn, Location, Rotation, SpawnParams);
		DroppedWeaponRef->WeaponDroppedEvent(SpawnWeaponData); //Set the pickups weapon stats 
		DroppedWeaponRef->GunMesh->AddImpulse(ThrowForce, NAME_None, true); // Throw the weapon
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GunToSpawn not set"));
	}
}

void AALSCustomCharacter::WeaponDropped() 
{
	CurrentWeaponData = EmptyWeaponData;
	PickupGunWBP(CurrentWeaponData);
	SetOverlayState(EALSOverlayState::Default);
}

void AALSCustomCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AALSCustomCharacter, CurrentWeapon);
	DOREPLIFETIME(AALSCustomCharacter, DeathOnce);
	DOREPLIFETIME_CONDITION(AALSCustomCharacter, CurrentWeaponData, COND_SkipOwner);

}


