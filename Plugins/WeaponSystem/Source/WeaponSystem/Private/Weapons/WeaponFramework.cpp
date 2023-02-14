// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponFramework.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AWeaponFramework::AWeaponFramework()
{
 	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	RootComponent = GunMeshComponent;
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	Muzzle->SetupAttachment(GunMeshComponent);
}

void AWeaponFramework::BeginPlay()
{
	Super::BeginPlay();
	// - Automatic Weapons will always start in Auto Mode
	WeaponData.bIsInAutoMode = WeaponData.bHasAutoMode;
	ActorsToIgnore.Emplace(this);
	ActorsToIgnore.Emplace(GetOwner());
	// todo Normalise the fire rate to an easier number//
	WeaponStats.FireRate = (WeaponStats.FireRate / 200.0f);
}


void AWeaponFramework::Fire(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy)
{
	ServerFire(in_Location, in_Rotation, ActorToIgnore, in_Accuracy);
}

// Formally Fire(); 
void AWeaponFramework::ServerFire_Implementation(const FVector in_Location, const FRotator in_Rotation,
	const AActor* ActorToIgnore, const float in_Accuracy)
{
	if(CanShoot())
	{
		TraceLocation = in_Location;
		TraceRotation = in_Rotation;
		TraceActorToIgnore = ActorToIgnore;
		AccuracyMultiplier = in_Accuracy;
		ServerShoot();
	}
	else
	{
		if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("Dry Fire"));}
		// WeaponEffects->PlayDryFire();
		// UGameplayStatics::PlaySoundAtLocation(this, DryFireSound, GetActorLocation());
		// PlayDryFire();
	}
}
// Formally Shoot();
void AWeaponFramework::ServerShoot_Implementation()
{
	if(!CanShoot())	{return;}
	ActorsToIgnore.Emplace(GetOwner());

	if(bDebuggingMode){WeaponData.CurrentAmmo++;}
	WeaponData.CurrentAmmo--;
	
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = LineTrace(Hit, ShotDirection);

	//- Defaults for Impact Effects
	//? FVector TracerEndPoint = Hit.TraceEnd;
	if(!bSuccess){UE_LOG(LogTemp,Warning,TEXT("Error in Linetrace"));return;}
	
	//- Check how close the gun is to the hit location
	FVector MuzzleLocation = Muzzle->GetComponentLocation();
	FVector NormalVector = (MuzzleLocation - Hit.Location);
	NormalVector.Normalize(0.0001);
	FVector ForwardVector = (FRotationMatrix(TraceRotation).GetScaledAxis( EAxis::X ));
	float VectorDistance = UKismetMathLibrary::Dot_VectorVector(NormalVector,(ForwardVector * -1));
	if(VectorDistance < 0.1)
	{
		//- If the gun is too close to the Hit location
		//- eg. gun against the wall 
		BlindFireWeapon();
	}
	else
	{
		//- Check if Anything is between the player and where they should be shooting //
		//- i.e. can see someone around the corner but gun is pointed at the wall //
		FHitResult HitCheck;
		ActorsToIgnore.Add(Hit.GetActor());

		//~ Debugging
		EDrawDebugTrace::Type DrawDebugType;
		if(bDebuggingMode){DrawDebugType = EDrawDebugTrace::ForDuration;}else{DrawDebugType = EDrawDebugTrace::None;}

		//- LineTrace, Did we hit something 
		if(UKismetSystemLibrary::LineTraceSingle(this,
			Hit.Location,
			MuzzleLocation,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
			true, ActorsToIgnore,
			DrawDebugType, HitCheck,
			true, FLinearColor::Blue,
			FLinearColor::Blue, 3.0f))
		{
			BlindFireWeapon();
		}
		else
		{
			//- Nothing in the way
			if(Hit.GetActor() != nullptr)
			{
				//todo: Fire in proper spot
				// APawn* OwnerPawn = Cast<APawn>(GetOwner());
				// ReceiveFire(Hit, OwnerPawn);
				ApplyDamageToActor(Hit, ShotDirection);
			}
		}
		ActorsToIgnore.Remove(Hit.GetActor()); //Remove as the var is kept
	}
	//todo temp
	// EPhysicalSurface SurfaceType = SurfaceType_Default;
	// SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
	//
	// PlayFireEffects(TracerEndPoint);
	// if(HasAuthority())
	// {
	// 	HitScanTrace.TraceTo = TracerEndPoint;
	// 	HitScanTrace.SurfaceType = SurfaceType;
	// 	HitScanTrace.a++;
	// }
}

bool AWeaponFramework::ServerShoot_Validate()
{
	return true;
}

// BUG When not in auto mode, Apply point damage will not add impulse to the actor??
void AWeaponFramework::ApplyDamageToActor(const FHitResult& Hit, FVector ShotDirection)
{
	
	//? FVector TracerEndPoint = Hit.TraceEnd;
	//? EPhysicalSurface SurfaceType;
	//? int SurfaceIndex;
	// todo Remove Old surface Method
	// todo Move all this stuff = not Damage related 
	// SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
	//New Method
	// SurfaceIndex = (Surfaces.Find(Hit.PhysMaterial.Get()));
	// if(bDebuggingMode)
	// {
	// 	UE_LOG(LogTemp,Warning, TEXT("Surface index: %i"), SurfaceIndex);
	// 	UE_LOG(LogTemp,Warning, TEXT("Surface %s"), ToCStr(Hit.PhysMaterial.Get()->GetName()));
	// }
	// if(HasAuthority())
	// {
	// 	HitScanTrace.TraceTo = TracerEndPoint;
	// 	HitScanTrace.SurfaceType = SurfaceType;
	// 	HitScanTrace.a++;
	// }
	//? FVector TracerEndPoint = Hit.ImpactPoint;
	//? PlayImpactEffects(SurfaceIndex, TracerEndPoint);
	// FVector ForwardVector = (FRotationMatrix(TraceRotation).GetScaledAxis( EAxis::X ));
	if(Hit.GetActor() == nullptr){return;}
	
	//? Hit.ImpactNormal * -1; - not as Realistic, but it's more satisfying, i.e. you shoot a box, the force will come from the face it was hit
	ShotDirection *= -1;
	UGameplayStatics::ApplyPointDamage(Hit.GetActor(), WeaponStats.DefaultDamage, ShotDirection, Hit,TraceActorToIgnore->GetInstigatorController(), this, WeaponStats.DamageType);

	// IALSCharacterInterface* HitCharacterInterface = Cast<IALSCharacterInterface>(Hit.GetActor());
	// if(HitCharacterInterface != nullptr)
	// {
	// 	//Hit a Player
	// 	HitCharacterInterface->BulletDamageEvent( GunWeaponStats.DefaultDamage, GunWeaponStats.HeadMultiplier, Hit.BoneName, TraceActorToIgnore->GetInstigatorController(), this, GunWeaponStats.DamageType);
	// 	// HitCharacterInterface->AddImpulseEvent((ForwardVector * GunWeaponStats.GunImpulse), Hit.BoneName, GunWeaponStats.GunImpulse);
	// 	HitCharacterInterface->AddImpulseEvent( Hit, GunWeaponStats.GunImpulse);
	// }
	// else
	// {
		// ShotDirection *= -1;
	// Hit.GetActor()
	// }
}




bool AWeaponFramework::LineTrace(FHitResult& Hit, FVector& ShotDirection) 
{
	const FVector ForwardVector = (FRotationMatrix(TraceRotation).GetScaledAxis( EAxis::X ));
	//? FVector NewBulletSpread;
	//? CalculateBulletSpread(NewBulletSpread);
	ShotDirection = -TraceRotation.Vector();
	const FVector LineEnd = ((ForwardVector * WeaponStats.GunRange) + TraceLocation);//? + NewBulletSpread;

	EDrawDebugTrace::Type DrawDebugType;
	if(bDebuggingMode){DrawDebugType = EDrawDebugTrace::ForDuration;}else{DrawDebugType = EDrawDebugTrace::None;}

	return UKismetSystemLibrary::LineTraceSingle(this,
		TraceLocation, LineEnd,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
	true, ActorsToIgnore,
	DrawDebugType, Hit, true,
	FLinearColor::Red,
	FLinearColor::Red, 1.0f);
}


void AWeaponFramework::Reload(float ReloadTime)
{
	if(WeaponData.CurrentAmmo >= WeaponData.ClipSize){return;}
	if(WeaponData.TotalAmmoCount <= 0){return;}
	if(bIsBeingReloaded){return;}
	ServerSetReloading(true);
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponFramework::ServerReload, ReloadTime, false);
}

void AWeaponFramework::ServerSetReloading_Implementation(bool bReloading)
{
	bIsBeingReloaded = bReloading;
}

void AWeaponFramework::ServerReload_Implementation()
{
	if(bIsBeingReloaded)
	{
		//- If we have enough ammo for another full clip // 
		if(WeaponData.TotalAmmoCount + WeaponData.CurrentAmmo > WeaponData.ClipSize)
		{
			//- Take out a full clip from the total ammo, (account for tactical reload)
			WeaponData.TotalAmmoCount = WeaponData.TotalAmmoCount - (WeaponData.ClipSize - WeaponData.CurrentAmmo);
			WeaponData.CurrentAmmo = WeaponData.ClipSize;
		}
		else //- Load however much ammo we have left into the clip
		{
			WeaponData.CurrentAmmo = WeaponData.CurrentAmmo + WeaponData.TotalAmmoCount;
			WeaponData.TotalAmmoCount = 0;
		}
	}
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	ServerSetReloading(false);
}

void AWeaponFramework::CancelReload()
{
	ServerSetReloading(false);
}

void AWeaponFramework::SwitchAutoMode()
{
	if(!WeaponData.bHasAutoMode){return;}
	WeaponData.bIsInAutoMode = !WeaponData.bIsInAutoMode;
}

void AWeaponFramework::BlindFireWeapon()
{
	if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("Blindfire - Weapon Framework"));}

}

void AWeaponFramework::MoveUMG(bool bIsRightShoulder)
{
	
}

void AWeaponFramework::FadeInUMG(bool bIsAiming)
{
	
}

bool AWeaponFramework::CanShoot() 
{
	if(bIsBeingReloaded) return false;
	if(WeaponData.CurrentAmmo <= 0) return false; 
	return true; 
}

