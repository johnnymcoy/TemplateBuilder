// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponFramework.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

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
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Fire - Weapon Framework "));}
	ServerFire(in_Location, in_Rotation, ActorToIgnore, in_Accuracy);
}

// Formally Fire(); 
void AWeaponFramework::ServerFire_Implementation(const FVector in_Location, const FRotator in_Rotation,
	const AActor* ActorToIgnore, const float in_Accuracy)
{
	if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("ServerFire Implementation"));}
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

//- Formally Shoot();
void AWeaponFramework::ServerShoot_Implementation()
{
	if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("ServerShoot Implementation"));}

	if(!CanShoot())	{return;}
	ActorsToIgnore.Emplace(GetOwner());

	if(bInfiniteAmmo){WeaponData.CurrentAmmo++;}
	WeaponData.CurrentAmmo--;
	
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = LineTrace(Hit, ShotDirection, FLinearColor::Red);

	//- Defaults for Impact Effects
	FVector TracerEndPoint = Hit.TraceEnd;
	
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

		//todo Change this to Linetrace Function, TraceLocation = Hit.Location
		// way to change just the end
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
			FLinearColor::Blue, DrawDebugTime))
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
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Current Ammo: %i"), WeaponData.CurrentAmmo);}
	
	// EPhysicalSurface SurfaceType = SurfaceType_Default;
	// SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

	//- v1 Replication? 
	// if(HasAuthority())
	// {
	// 	HitScanTrace.TraceTo = TracerEndPoint;
	// 	// HitScanTrace.SurfaceType = SurfaceType;
	// 	HitScanTrace.a++;
	//
	// }
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

// bool AWeaponFramework::ServerLineTrace_Implementation(FHitResult& Hit, FVector& ShotDirection, FLinearColor Color,
// 	FVector CustomLineEnd)
// {
// 	LineTrace(Hit,ShotDirection,Color, CustomLineEnd);
// }

bool AWeaponFramework::LineTrace(FHitResult& Hit, FVector& ShotDirection, FLinearColor Color, FVector CustomLineEnd) 
{
	const FVector ForwardVector = (FRotationMatrix(TraceRotation).GetScaledAxis( EAxis::X ));
	FVector BulletSpread;
	CalculateBulletSpread(BulletSpread);
	ShotDirection = -TraceRotation.Vector();
	const FVector LineEnd = ((ForwardVector * WeaponStats.GunRange) + TraceLocation) + BulletSpread;

	EDrawDebugTrace::Type DrawDebugType;
	if(bDebuggingMode){DrawDebugType = EDrawDebugTrace::ForDuration;}else{DrawDebugType = EDrawDebugTrace::None;}

	return UKismetSystemLibrary::LineTraceSingle(
		this,
		TraceLocation,
		LineEnd,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
	true,
		ActorsToIgnore,
		DrawDebugType,
		Hit,
		true,
	Color,	Color,
	DrawDebugTime);
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

void AWeaponFramework::CalculateBulletSpread(FVector& NewBulletSpread)
{
	float OwnerSpeed = GetOwner()->GetVelocity().Size();
	//- Still // 
	if(OwnerSpeed < 10)
	{
		if(WeaponData.bHasAutoMode && !WeaponData.bIsInAutoMode) {OwnerSpeed = 0.75f;}
		else{OwnerSpeed = 1.0f;}
	}
	//- Moving // 
	else
	{
		if(WeaponData.bHasAutoMode && !WeaponData.bIsInAutoMode) {OwnerSpeed = 1.5f;}
		else{OwnerSpeed = 1.75f;}
	}
	OwnerSpeed = (OwnerSpeed / AccuracyMultiplier);
	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Owner Speed Multipler: %f"), OwnerSpeed);}
	
	// - Random Variation to X and Y axis //
	const float Spread = WeaponStats.BulletSpread * OwnerSpeed; 
	const float BulletX = FMath::RandRange((Spread * -1), Spread);
	const float BulletY = FMath::RandRange((Spread * -1), Spread);
	const float BulletZ = FMath::RandRange((Spread * -1), Spread);
	NewBulletSpread = FVector(BulletX,BulletY,BulletZ);
}

// void AWeaponFramework::OnRep_HitScanTrace()
// {
// 	if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("On Rep, HitScan Trace"));}
// }

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
		if(bDebuggingMode){UE_LOG(LogWeaponSystem, Warning, TEXT("Current Ammo: %i"), WeaponData.CurrentAmmo);}
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
	//! v2
	//- Linetrace but from the MuzzleLocation //
	//- V2 has the LineEnd calculated from the Muzzle Location  // 
	TraceLocation = Muzzle->GetComponentLocation();
	FHitResult BlindFireHit;
	FVector ShotDirection;
	LineTrace(BlindFireHit, ShotDirection, FLinearColor::Green);
	
	//! v1
	// const FVector ForwardVector = (FRotationMatrix(TraceRotation).GetScaledAxis( EAxis::X ));
	// FVector BulletSpread;
	// CalculateBulletSpread(BulletSpread);
	// const FVector LineEnd = ((ForwardVector * GunRange) + TraceLocation) + BulletSpread;
	// const FVector ShotDirection = -TraceRotation.Vector();
	// const FVector MuzzleLocation = Muzzle->GetComponentLocation();
	// FHitResult BlindFireHit;
	// EDrawDebugTrace::Type DrawDebugType;
	// if(bDebuggingMode){DrawDebugType = EDrawDebugTrace::ForDuration;}else{DrawDebugType = EDrawDebugTrace::None;}
	// UKismetSystemLibrary::LineTraceSingle(this, MuzzleLocation, LineEnd,
	// 	UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
	// 	true, ActorsToIgnore,
	// 	DrawDebugType, BlindFireHit,
	// 	true, FLinearColor::Green,
	// 	FLinearColor::Green, DrawDebugTime
	// 	);
	//- Apply Damage //
	if(BlindFireHit.GetActor() != nullptr)
	{
		ApplyDamageToActor(BlindFireHit, ShotDirection);
	}
}

void AWeaponFramework::MoveUMG(bool bIsRightShoulder)
{
	
}

void AWeaponFramework::FadeInUMG(bool bIsAiming)
{
	
}

void AWeaponFramework::MultiCastSetWeaponMesh_Implementation(USkeletalMesh* SkeletalMesh)
{
	GunMeshComponent->SetSkeletalMesh(SkeletalMesh);
}

void AWeaponFramework::SetWeaponMesh_Implementation(USkeletalMesh* SkeletalMesh)
{
	GunMeshComponent->SetSkeletalMesh(SkeletalMesh);
	MultiCastSetWeaponMesh(SkeletalMesh);
}

bool AWeaponFramework::CanShoot() 
{
	if(bIsBeingReloaded) return false;
	if(WeaponData.CurrentAmmo <= 0) return false; 
	return true; 
}

// void AWeaponFramework::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
// 	//todo not it.. 
// 	// DOREPLIFETIME(AWeaponFramework, bDebuggingMode);
//
// 	// DOREPLIFETIME_CONDITION(AWeaponFramework, HitScanTrace, COND_SkipOwner);
// 	// DOREPLIFETIME(AWeaponBase, TraceLocation);
// 	// DOREPLIFETIME(AWeaponBase, GunWeaponData);
// 	// DOREPLIFETIME(AWeaponBase, GunWeaponStats);
// }
//
