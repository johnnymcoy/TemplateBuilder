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
	if(GunWeaponData.bHasAutoMode)	{GunWeaponData.bIsInAutoMode = true;}
	// Normalise the fire rate to an easier number//
	GunWeaponStats.FireRate = (GunWeaponStats.FireRate / 200.0f);
	// FireRate = (FireRate / 200.f); 
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
		if(!bIsReloading)
		{
			ServerSetReloading(true);
			GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeaponBase::ServerReload, ReloadTime, false);
		}
	} 
}

void AWeaponBase::CancelReload_Implementation() 
{
	ServerSetReloading(false);
}

void AWeaponBase::ServerSetReloading_Implementation(bool bReloading) 
{
	bIsReloading = bReloading;
}

void AWeaponBase::ServerReload_Implementation() 
{
	if(bIsReloading)
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
	if(GunWeaponData.bHasAutoMode)
	{
		if(GunWeaponData.bIsInAutoMode)
		{
			GunWeaponData.bIsInAutoMode = false;
		}
		else
		{
			GunWeaponData.bIsInAutoMode = true;
		}
	}
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
			//Old Surface Method todo remove
			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
			//New Method
			SurfaceIndex = (Surfaces.Find(Hit.PhysMaterial.Get()));
			UE_LOG(LogTemp,Warning, TEXT("Surface index: %i"), SurfaceIndex);
			UE_LOG(LogTemp,Warning, TEXT("Surface %s"), ToCStr(Hit.PhysMaterial.Get()->GetName()));

			// Check how close the gun is to the hit location
			FVector MuzzleLocation = Muzzle->GetComponentLocation();
			FVector NormalVector = (MuzzleLocation - Hit.Location);
			NormalVector.Normalize(0.0001);
			FVector ForwardVector = (FRotationMatrix(TraceRotation).GetScaledAxis( EAxis::X ));
			float VectorDistance = UKismetMathLibrary::Dot_VectorVector(NormalVector,(ForwardVector * -1));
			if(VectorDistance < 0.1)
			{
				//If the gun is too close to the Hit location
				BlindFireWeapon();
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
					BlindFireWeapon();
					//There's something in the way (from the camera to where to shoot)
					// Blindfire from the gun instead					
					if(bDebuggingMode){DrawDebugPoint(GetWorld(), HitCheck.Location, 20, FColor::Green, true);}
				}
				else
				{
					//Nothing in the way
					if(Hit.GetActor() != nullptr)
					{
						//todo Remove BulletDamageEvent + Add impulseEvent 
						//todo: pointdamage change
						IALSCharacterInterface* HitCharacterInterface = Cast<IALSCharacterInterface>(Hit.GetActor());
						if(HitCharacterInterface != nullptr)
						{
							//Hit a Player
							HitCharacterInterface->Execute_BulletDamageEvent(Hit.GetActor(), GunWeaponStats.DefaultDamage, GunWeaponStats.HeadMultiplier, Hit.BoneName, TraceActorToIgnore->GetInstigatorController(), this, GunWeaponStats.DamageType);
							HitCharacterInterface->Execute_AddImpulseEvent(Hit.GetActor(), (ForwardVector * GunWeaponStats.GunImpulse), Hit.BoneName, GunWeaponStats.GunImpulse);
							//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Green, true);
						}
						else
						{
							//Direction Is what dictates the Impulse
							ShotDirection *= -1;
							UGameplayStatics::ApplyPointDamage(Hit.GetActor(), GunWeaponStats.DefaultDamage, ShotDirection, Hit,TraceActorToIgnore->GetInstigatorController(), this, GunWeaponStats.DamageType);
							DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Purple, true);
						}
					}
					if(bDebuggingMode){DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Blue, true);}
				}
			}
			//todo New function 
			TracerEndPoint = Hit.ImpactPoint;
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

void AWeaponBase::BlindFireWeapon()
{
	const FVector ForwardVector = (FRotationMatrix(TraceRotation).GetScaledAxis( EAxis::X ));
	FVector NewBulletSpread;
	CalculateBulletSpread(NewBulletSpread);
	const FVector LineEnd = ((ForwardVector * GunRange) + TraceLocation) + NewBulletSpread;
	FVector ShotDirection = -TraceRotation.Vector();

	FVector MuzzleLocation = Muzzle->GetComponentLocation();
	FHitResult BlindFireHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(TraceActorToIgnore);
	GetWorld()->LineTraceSingleByChannel(BlindFireHit, MuzzleLocation, LineEnd, ECollisionChannel::ECC_GameTraceChannel4, Params);
	if(bDebuggingMode){DrawDebugPoint(GetWorld(), BlindFireHit.Location, 20, FColor::Yellow, true);}
	if(BlindFireHit.GetActor() != nullptr)
	{
		//todo Remove BulletDamageEvent + Add impulseEvent 
		//todo: pointdamage change
		IALSCharacterInterface* HitCharacterInterface = Cast<IALSCharacterInterface>(BlindFireHit.GetActor());
		if(HitCharacterInterface != nullptr)
		{
			//Hit a Player
			HitCharacterInterface->Execute_BulletDamageEvent(BlindFireHit.GetActor(), GunWeaponStats.DefaultDamage, GunWeaponStats.HeadMultiplier, BlindFireHit.BoneName, TraceActorToIgnore->GetInstigatorController(), this, GunWeaponStats.DamageType);
			HitCharacterInterface->Execute_AddImpulseEvent(BlindFireHit.GetActor(), (ForwardVector * GunWeaponStats.GunImpulse), BlindFireHit.BoneName, GunWeaponStats.GunImpulse);
			//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Green, true);
		}
		else
		{
			//Direction Is what dictates the Impulse
			ShotDirection *= -1;
			UGameplayStatics::ApplyPointDamage(BlindFireHit.GetActor(), GunWeaponStats.DefaultDamage, ShotDirection, BlindFireHit,TraceActorToIgnore->GetInstigatorController(), this, GunWeaponStats.DamageType);
			DrawDebugPoint(GetWorld(), BlindFireHit.Location, 20, FColor::Purple, true);
		}
	}

	
}

void AWeaponBase::ServerShoot_Implementation() 
{
	Fire();
}

bool AWeaponBase::ServerShoot_Validate() 
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
		if(GunWeaponData.bHasAutoMode && !GunWeaponData.bIsInAutoMode)	{OwnerSpeed = 0.75f;}
		else							{OwnerSpeed = 1.0f;	}
	}
	else				//Moving
	{
		if(GunWeaponData.bHasAutoMode && !GunWeaponData.bIsInAutoMode)	{OwnerSpeed = 1.5f;}
		else							{OwnerSpeed = 1.75f;}
	}
	OwnerSpeed = (OwnerSpeed / AccuracyMultiplier);
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Owner Speed Multiplier: %f"), OwnerSpeed);}
	//Adds some random Variation to both X and Y axis
	float BulletX = FMath::RandRange((GunWeaponStats.BulletSpread * -1 * OwnerSpeed), (GunWeaponStats.BulletSpread * OwnerSpeed));
	float BulletY = FMath::RandRange((GunWeaponStats.BulletSpread * -1 * OwnerSpeed), (GunWeaponStats.BulletSpread * OwnerSpeed));
	float BulletZ = FMath::RandRange((GunWeaponStats.BulletSpread * -1 * OwnerSpeed), (GunWeaponStats.BulletSpread * OwnerSpeed));
	NewBulletSpread = FVector(BulletX, BulletY, BulletZ);
}

bool AWeaponBase::CanShoot() 
{
	if(bIsReloading) return false;
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
	DOREPLIFETIME(AWeaponBase, GunWeaponStats);
}
