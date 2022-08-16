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
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	// Separate Component?
	// AmmoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmmoWidget"));
	// AmmoWidgetComponent->SetupAttachment(GunMeshComponent);
	// AmmoWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// AmmoWidgetComponent->SetWidget(WeaponWidget);
	// UMG_RightLocation = CreateDefaultSubobject<USceneComponent>(TEXT("UMGRightLocation"));
	// UMG_RightLocation->SetupAttachment(GunMeshComponent);
	// UMG_LeftLocation = CreateDefaultSubobject<USceneComponent>(TEXT("UMGLeftLocation"));
	// UMG_LeftLocation->SetupAttachment(GunMeshComponent);


	//todo Weapon Attachments

	FlashlightAttachment = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flashlight"));
	FlashlightAttachment->SetupAttachment(GunMeshComponent);
	FlashlightAttachment->SetVisibility(false);
	FlashlightAttachment->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AttachmentSlot01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttachmentShot01"));
	AttachmentSlot01->SetupAttachment(GunMeshComponent);
	AttachmentSlot01->SetVisibility(false);
	AttachmentSlot01->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AttachmentSlot02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttachmentShot02"));
	AttachmentSlot02->SetupAttachment(GunMeshComponent);
	AttachmentSlot02->SetVisibility(false);
	AttachmentSlot02->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AttachmentSlot03 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttachmentShot03"));
	AttachmentSlot03->SetupAttachment(GunMeshComponent);
	AttachmentSlot03->SetVisibility(false);
	AttachmentSlot03->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	DrawDebugType = EDrawDebugTrace::None;
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

void AWeaponBase::Fire() 
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

void AWeaponBase::Reload(float ReloadTime) 
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

void AWeaponBase::CancelReload() 
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

void AWeaponBase::SwitchAutoMode() 
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
void AWeaponBase::MoveUMG(bool bIsRightShoulder) 
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

void AWeaponBase::FadeInUMG(bool bIsAiming) 
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

void AWeaponBase::GetTraceParams(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy) 
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
	if(bDebuggingMode){DrawDebugType = EDrawDebugTrace::ForDuration;}else{DrawDebugType = EDrawDebugTrace::None;}
	return UKismetSystemLibrary::LineTraceSingle(this, TraceLocation, LineEnd, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
	true, ActorsToIgnore, DrawDebugType, Hit, true, FLinearColor::Red, FLinearColor::Red, 1.0f);
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
		if(bSuccess)
		{
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
				ActorsToIgnore.Add(Hit.GetActor());
				if(bDebuggingMode){DrawDebugType = EDrawDebugTrace::ForDuration;}else{DrawDebugType = EDrawDebugTrace::None;}
				if(UKismetSystemLibrary::LineTraceSingle(this, Hit.Location, MuzzleLocation, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
							true, ActorsToIgnore, DrawDebugType, HitCheck, true, FLinearColor::Blue, FLinearColor::Blue, 1.0f))
				{
					BlindFireWeapon();
				}
				else
				{
					//Nothing in the way
					if(Hit.GetActor() != nullptr)
					{
						//todo: Fire in proper spot
						APawn* OwnerPawn = Cast<APawn>(GetOwner());
						ReceiveFire(Hit, OwnerPawn);
						ApplyDamageToActor(Hit, ShotDirection);
					}
				}
				ActorsToIgnore.Remove(Hit.GetActor()); //Remove as the var is kept
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Error in Linetrace"));
		}
		//todo temp
		EPhysicalSurface SurfaceType = SurfaceType_Default;
		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

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
	if(bDebuggingMode){DrawDebugType = EDrawDebugTrace::ForDuration;}else{DrawDebugType = EDrawDebugTrace::None;}
	UKismetSystemLibrary::LineTraceSingle(this, MuzzleLocation, LineEnd, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
					true, ActorsToIgnore, DrawDebugType, BlindFireHit, true, FLinearColor::Green, FLinearColor::Green, 1.0f);
	if(BlindFireHit.GetActor() != nullptr)
	{
		ApplyDamageToActor(BlindFireHit, ShotDirection);
	}
}

void AWeaponBase::ToggleFlashlight()
{
	UE_LOG(LogTemp,Warning,TEXT("Flashlight toggle"));
}

void AWeaponBase::ApplyDamageToActor(const FHitResult& Hit, FVector ShotDirection)
{
	FVector TracerEndPoint = Hit.TraceEnd;
	EPhysicalSurface SurfaceType = SurfaceType_Default;
	int SurfaceIndex = 0;
	//Old Surface Method todo remove
	SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
	//New Method
	SurfaceIndex = (Surfaces.Find(Hit.PhysMaterial.Get()));
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

	TracerEndPoint = Hit.ImpactPoint;
	PlayImpactEffects(SurfaceIndex, TracerEndPoint);

	FVector ForwardVector = (FRotationMatrix(TraceRotation).GetScaledAxis( EAxis::X ));
	if(Hit.GetActor() != nullptr)
	{
		//todo: pointdamage change
		IALSCharacterInterface* HitCharacterInterface = Cast<IALSCharacterInterface>(Hit.GetActor());
		if(HitCharacterInterface != nullptr)
		{
			//Hit a Player
			HitCharacterInterface->BulletDamageEvent( GunWeaponStats.DefaultDamage, GunWeaponStats.HeadMultiplier, Hit.BoneName, TraceActorToIgnore->GetInstigatorController(), this, GunWeaponStats.DamageType);
			// HitCharacterInterface->AddImpulseEvent((ForwardVector * GunWeaponStats.GunImpulse), Hit.BoneName, GunWeaponStats.GunImpulse);
			HitCharacterInterface->AddImpulseEvent( Hit, GunWeaponStats.GunImpulse);
		}
		else
		{
			ShotDirection *= -1;
			UGameplayStatics::ApplyPointDamage(Hit.GetActor(), GunWeaponStats.DefaultDamage, ShotDirection, Hit,TraceActorToIgnore->GetInstigatorController(), this, GunWeaponStats.DamageType);
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
