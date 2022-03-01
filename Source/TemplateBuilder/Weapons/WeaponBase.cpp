// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "DrawDebugHelpers.h"
#include "TemplateBuilder/Characters/ALSCharacterInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "TemplateBuilder/HUD/WeaponWidget.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComponent"));
	RootComponent = GunMeshComponent;
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(GunMeshComponent);

	
	// Separate Component?
	// AmmoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmmoWidget"));
	// AmmoWidgetComponent->SetupAttachment(GunMeshComponent);
	// AmmoWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// // AmmoWidgetComponent->SetWidget(WeaponWidget);
	// UMG_RightLocation = CreateDefaultSubobject<USceneComponent>(TEXT("UMGRightLocation"));
	// UMG_RightLocation->SetupAttachment(GunMeshComponent);
	// UMG_LeftLocation = CreateDefaultSubobject<USceneComponent>(TEXT("UMGLeftLocation"));
	// UMG_LeftLocation->SetupAttachment(GunMeshComponent);
	
	bReplicates = true;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();	
	// Normalise the fire rate to an easier number//
	FireRate = (FireRate / 200.f); 
	if(HasAutoMode)	{InAutoMode = true;}
}

void AWeaponBase::Fire_Implementation() 
{
	if(CanShoot())
	{
		Shoot();
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(this, DryFireSound, GetActorLocation());
	}	
}

void AWeaponBase::Reload_Implementation(float ReloadTime) 
{
	if(GunWeaponData.CurrentAmmo < GunWeaponData.ClipSize && GunWeaponData.TotalAmmoCount > 0)
	{
		if(!IsReloading)
		{
			ServerSetReloading(true);
			GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeaponBase::ServerReload, ReloadTime, false);
			//Update Ammo UMG
		}
	} 
}

void AWeaponBase::CancelReload_Implementation() 
{
	ServerSetReloading(false);
}

void AWeaponBase::ServerSetReloading_Implementation(bool bReloading) 
{
	IsReloading = bReloading;
}

void AWeaponBase::ServerReload_Implementation() 
{
	if(IsReloading)
	{
		if(GunWeaponData.TotalAmmoCount + GunWeaponData.CurrentAmmo > GunWeaponData.ClipSize)
		{
			GunWeaponData.TotalAmmoCount = GunWeaponData.TotalAmmoCount - (GunWeaponData.ClipSize - GunWeaponData.CurrentAmmo);
			GunWeaponData.CurrentAmmo = GunWeaponData.ClipSize;
		}
		else
		{
			GunWeaponData.CurrentAmmo = GunWeaponData.CurrentAmmo + GunWeaponData.TotalAmmoCount;
			GunWeaponData.TotalAmmoCount = 0;
		}
	}
	GetWorldTimerManager().ClearTimer(ReloadTimer);
	ServerSetReloading(false);
}

void AWeaponBase::SwitchAutoMode_Implementation() 
{
	if(HasAutoMode)
	{
		if(InAutoMode)
		{
			InAutoMode = false;
		}
		else
		{
			InAutoMode = true;
		}
	}
}

FWeaponData AWeaponBase::GetWeaponData_Implementation() 
{
	return GunWeaponData;
}

	//Separate Component
//////////////////////////////////
void AWeaponBase::MoveUMG_Implementation(bool bIsRightShoulder) 
{
	// if(bIsRightShoulder)
	// {
	// 	AmmoWidgetComponent->SetWorldLocation(UMG_LeftLocation->GetComponentLocation());
	// }
	// else
	// {
	// 	AmmoWidgetComponent->SetWorldLocation(UMG_RightLocation->GetComponentLocation());
	// }
}

void AWeaponBase::FadeInUMG_Implementation(bool bIsAiming) 
{
	// if(bIsAiming && UMGAlpha <= 0.3)
	// {
	// 	GetWorldTimerManager().SetTimer(UMGTimer, this, &AWeaponBase::FadeUMGIn, 0.08, true, 0.0f);
	// }
	// else
	// {
	// 	GetWorldTimerManager().SetTimer(UMGTimer, this, &AWeaponBase::FadeUMGOut, 0.08, true, 0.0f);
	// }
}

void AWeaponBase::FadeUMGIn() 
{
	// if(UMGAlpha >= 0.3)
	// {
	// 	GetWorldTimerManager().ClearTimer(UMGTimer);
	// 	return;
	// }
	// UMGAlpha += 0.04f;
	// FadeInUMGTimed(UMGAlpha);
}

void AWeaponBase::FadeUMGOut() 
{
	// if(UMGAlpha <= 0) 
	// {
	// 	GetWorldTimerManager().ClearTimer(UMGTimer);
	// 	return;
	// }
	// UMGAlpha -= 0.04f;
	// FadeInUMGTimed(UMGAlpha);
}

void AWeaponBase::FadeInUMGTimed(float Alpha) 
{
	// AmmoWidgetComponent->SetTintColorAndOpacity(FLinearColor(1,1,1,Alpha));
	// if(Alpha >= 0.3 || Alpha <= 0)
	// {
	// 	GetWorldTimerManager().ClearTimer(UMGTimer);
	// }
}

void AWeaponBase::GetTraceParams_Implementation(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy) 
{
	ServerGetTraceParams(in_Location, in_Rotation, ActorToIgnore, in_Accuracy);
}

void AWeaponBase::ServerGetTraceParams_Implementation(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy) 
{
	TraceLocation = in_Location;
	TraceRotation = in_Rotation;
	TraceActorToIgnore = ActorToIgnore;
	AccuracyMultiplier = in_Accuracy;
	ServerShoot();
}

void AWeaponBase::EnableWeaponDebug_Implementation(bool DebugStatus) 
{
	bDebuggingMode = DebugStatus;
}

bool AWeaponBase::IsInAutoMode_Implementation() 
{
	return InAutoMode;
}

void AWeaponBase::SetWeaponData_Implementation(const FWeaponData in_WeaponData) 
{
	GunWeaponData = in_WeaponData;
}

bool AWeaponBase::LineTrace(FHitResult& Hit, FVector& ShotDirection) 
{
	const FVector ForwardVector = (FRotationMatrix(TraceRotation).GetScaledAxis( EAxis::X ));
	FVector NewBulletSpread;
	CalculateBulletSpread(NewBulletSpread);
	ShotDirection = -TraceRotation.Vector();
	const FVector LineEnd = ((ForwardVector * GunRange) + TraceLocation) + NewBulletSpread;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
	if(bDebuggingMode)
	{return UKismetSystemLibrary::LineTraceSingle(this, TraceLocation, LineEnd, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
	true, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Red, FLinearColor::Green, 0.5f);}
	else
	{return 	UKismetSystemLibrary::LineTraceSingle(this, TraceLocation, LineEnd, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
	true, ActorsToIgnore, EDrawDebugTrace::None, Hit, true, FLinearColor::Red, FLinearColor::Green, 0.5f);}
}

//TraceParams Already calls Server shoot
void AWeaponBase::Shoot() 
{
	if(CanShoot())
	{
		GunWeaponData.CurrentAmmo--;
		if(bDebuggingMode){GunWeaponData.CurrentAmmo++;}
		FHitResult Hit;
		FVector ShotDirection;
		bool bSuccess = LineTrace(Hit, ShotDirection);
		//Defaults for Impact effects
		FVector TracerEndPoint = Hit.TraceEnd;
		EPhysicalSurface SurfaceType = SurfaceType_Default;
		int SurfaceIndex = 0;
		if(bSuccess)
		{
			if(HasAuthority())	{UE_LOG(LogTemp,Warning,TEXT("ServerShoot"));}
			//Old Method todo remove
			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
			//New Method
			SurfaceIndex = (Surfaces.Find(Hit.PhysMaterial.Get()));
			UE_LOG(LogTemp,Warning, TEXT("Surface index: %i"), SurfaceIndex);
			UE_LOG(LogTemp,Warning, TEXT("Surface %s"), ToCStr(Hit.PhysMaterial.Get()->GetName()));
			
			FVector ForwardVector = (FRotationMatrix(TraceRotation).GetScaledAxis( EAxis::X ));
			if(Hit.GetActor() != nullptr)
			{
				IALSCharacterInterface* HitCharacterInterface = Cast<IALSCharacterInterface>(Hit.GetActor());
				if(HitCharacterInterface != nullptr)
				{
					//Hit a Player
					// ServerBulletDamage(Hit, (ForwardVector * GunImpulse));
					HitCharacterInterface->Execute_BulletDamageEvent(Hit.GetActor(), DefaultDamage, HeadMultiplier, Hit.BoneName, TraceActorToIgnore->GetInstigatorController(), this, DamageType);
					HitCharacterInterface->Execute_AddImpulseEvent(Hit.GetActor(), (ForwardVector * GunImpulse), Hit.BoneName, GunImpulse);
					//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Green, true);
				}
				// else
				// {
				// 	//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Purple, true);
				// }
			}
			// needs more work
			FVector MuzzleLocation = Muzzle->GetComponentLocation();
			FVector NormalVector = (Hit.Location - MuzzleLocation);
			NormalVector.Normalize(0.0001);
			float VectorDistance = UKismetMathLibrary::Dot_VectorVector(NormalVector,(ForwardVector * -1));
			if(VectorDistance < 0.1)
			{
				//If the gun is too close to the Hit location
				FHitResult BlindFireHit;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(this);
				Params.AddIgnoredActor(GetOwner());
				Params.AddIgnoredActor(TraceActorToIgnore);
				GetWorld()->LineTraceSingleByChannel(BlindFireHit, MuzzleLocation, Hit.TraceEnd, ECollisionChannel::ECC_GameTraceChannel4, Params);
				if(bDebuggingMode){DrawDebugPoint(GetWorld(), BlindFireHit.Location, 20, FColor::Red, true);}
			}
			else
			{
				//Check if Anything is between the player and where they should be shooting //
				//i.e. can see someone around the corner but gun is pointed at the wall //
				FHitResult HitCheck;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(this);
				Params.AddIgnoredActor(GetOwner());
				Params.AddIgnoredActor(Hit.Actor.Get());
				Params.AddIgnoredActor(TraceActorToIgnore);

				if(GetWorld()->LineTraceSingleByChannel(HitCheck, Hit.Location, MuzzleLocation, ECollisionChannel::ECC_GameTraceChannel4, Params))
				{
					//There's something in the way 
					// Blindfire from the gun instead					
					if(bDebuggingMode){DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Green, true);}


				}
				else
				{
					if(bDebuggingMode){DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Blue, true);}
				}
			}
			TracerEndPoint = Hit.ImpactPoint;
			//NEW
			PlayImpactEffects(SurfaceIndex, TracerEndPoint);
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Error in Linetrace"));
		}
		PlayFireEffects(TracerEndPoint);
		if(HasAuthority())
		{
			HitScanTrace.TraceTo = TracerEndPoint;
			HitScanTrace.SurfaceType = SurfaceType;
			HitScanTrace.a++;
		}
	}
}

void AWeaponBase::ServerShoot_Implementation() 
{
	Fire();
	// Shoot();
}

bool AWeaponBase::ServerShoot_Validate() 
{
	return true;
}

void AWeaponBase::ServerBulletDamage_Implementation(FHitResult Hit, FVector in_Impulse) 
{
	UGameplayStatics::ApplyDamage(Hit.GetActor(), DefaultDamage, TraceActorToIgnore->GetInstigatorController(), this, DamageType);
	// IALSCharacterInterface* HitCharacterInterface = Cast<IALSCharacterInterface>(Hit.GetActor());
	// if(HitCharacterInterface != nullptr)
	// {
	// 	// HitCharacterInterface->Execute_BulletDamageEvent(Hit.GetActor(), DefaultDamage, HeadMultiplier, Hit.BoneName, TraceActorToIgnore->GetInstigatorController(), this, DamageType);
	// 	// HitCharacterInterface->Execute_AddImpulseEvent(Hit.GetActor(), in_Impulse, Hit.BoneName, GunImpulse);
	// 	UE_LOG(LogTemp,Warning, TEXT("HitPlayerServer"));
	// }
}

bool AWeaponBase::ServerBulletDamage_Validate(FHitResult Hit, FVector in_Impulse) 
{
	return true;
}

void AWeaponBase::PlayFireEffects(FVector TraceEnd) 
{
	if(FireParticles != nullptr)
	{
		UGameplayStatics::SpawnEmitterAttached(FireParticles, Muzzle);
	}
	//Spawn Tracer Round
}

void AWeaponBase::PlayImpactEffects(int SurfaceType, FVector ImpactPoint) 
{
	FVector MuzzleLocation = Muzzle->GetComponentLocation();
	FVector ShotDirection = ImpactPoint - MuzzleLocation;
	ShotDirection.Normalize();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSelector(SurfaceType), ImpactPoint, ShotDirection.Rotation());
}

void AWeaponBase::OnRep_HitScanTrace() 
{
	PlayFireEffects(HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}


void AWeaponBase::CalculateBulletSpread(FVector& NewBulletSpread)
{
	float OwnerSpeed = GetOwner()->GetVelocity().Size();
	if(OwnerSpeed < 10) //Still
	{
		if(HasAutoMode && !InAutoMode)	{OwnerSpeed = 0.75f;}
		else							{OwnerSpeed = 1.0f;	}
	}
	else				//Moving
	{
		if(HasAutoMode && !InAutoMode)	{OwnerSpeed = 1.5f;}
		else							{OwnerSpeed = 1.75f;}
	}
	OwnerSpeed = (OwnerSpeed / AccuracyMultiplier);
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Owner Speed Multiplier: %f"), OwnerSpeed);}
	//Adds some random Variation to both X and Y axis
	float BulletX = FMath::RandRange((BulletSpread * -1 * OwnerSpeed), (BulletSpread * OwnerSpeed));
	float BulletY = FMath::RandRange((BulletSpread * -1 * OwnerSpeed), (BulletSpread * OwnerSpeed));
	float BulletZ = FMath::RandRange((BulletSpread * -1 * OwnerSpeed), (BulletSpread * OwnerSpeed));
	NewBulletSpread = FVector(BulletX, BulletY, BulletZ);
}

bool AWeaponBase::CanShoot() 
{
	if(IsReloading) return false;
	if(GunWeaponData.CurrentAmmo <= 0) return false; 
	return true; 
}

UParticleSystem* AWeaponBase::ParticleSelector(const int in_SurfaceType)
{
	
	switch(in_SurfaceType)
	{
	case -1: //Default Ground
		return ImpactEffects.General01;
	case 0: //Dirt
		return ImpactEffects.Dirt;
	case 1: //General 01
		return ImpactEffects.General01;
	case 2: //General 02
		return ImpactEffects.General02;
	case 3: //Grass
		return ImpactEffects.Grass;
	case 4: //Gravel
		return ImpactEffects.Gravel;
	case 5: //Ice
		return ImpactEffects.Ice;
	case 6: //Leaves
		return ImpactEffects.Leaves;
	case 7: //Mud
		return ImpactEffects.Mud;
	case 8: //Sand
		return ImpactEffects.Sand;
	case 9: //SnowHeavy
		return ImpactEffects.SnowHeavy;
	case 10: //SnowLight
		return ImpactEffects.SnowLight;
	case 11: //Sparks
		return ImpactEffects.Sparks;
	case 12: //SpecialAbility
		return ImpactEffects.SpecialAbility;
	case 13: //WaterHeavy
		return ImpactEffects.WaterHeavy;
	case 14: //WaterLight
		return ImpactEffects.WaterLight;
	case 15: //Carpet
		return ImpactEffects.Carpet;
	case 16: //Concrete
		return ImpactEffects.Concrete;
	case 17: //Metal
		return ImpactEffects.Metal;
	case 18: //Plastic
		return ImpactEffects.Plastic;
	case 19: //Wood
		return ImpactEffects.Wood;
	case 20: //Flesh
		return ImpactEffects.Flesh;
	default:
		return ImpactEffects.General01;
	}
}


void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 	DOREPLIFETIME_CONDITION(AWeaponBase, HitScanTrace, COND_SkipOwner);
	DOREPLIFETIME(AWeaponBase, TraceLocation);
	DOREPLIFETIME(AWeaponBase, GunWeaponData);
}
