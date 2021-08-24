// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "DrawDebugHelpers.h"
#include "TemplateBuilder/Characters/ALSCharacterInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "TimerManager.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComponent"));
	RootComponent = GunMeshComponent;
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(GunMeshComponent);

	AmmoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmmoWidget"));
	AmmoWidgetComponent->SetupAttachment(GunMeshComponent);
	AmmoWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UMG_RightLocation = CreateDefaultSubobject<USceneComponent>(TEXT("UMGRightLocation"));
	UMG_RightLocation->SetupAttachment(GunMeshComponent);
	UMG_LeftLocation = CreateDefaultSubobject<USceneComponent>(TEXT("UMGLeftLocation"));
	UMG_LeftLocation->SetupAttachment(GunMeshComponent);
	
	//Multiplayer
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

float AWeaponBase::Reload_Implementation() 
{
	if(CurrentAmmoCount < ClipSize && TotalAmmoCount > 0)
	{
		if(!IsReloading)
		{
			IsReloading = true;
			//Play Gun Reload Animation at speed dependant on Reload time
			GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeaponBase::ReloadDelay, ReloadTime, false);
			//Update Ammo UMG
		}
	} 
	//Needs an Offset to make sure it's after the ReloadDelay Function
	return (ReloadTime + 0.1f);
}

void AWeaponBase::CancelReload_Implementation() 
{
	IsReloading = false;
}

void AWeaponBase::ServerReload_Implementation() 
{
	ReloadDelay();
}

void AWeaponBase::ReloadDelay() 
{
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerReload();
	}
	if(IsReloading)
	{
		if(TotalAmmoCount + CurrentAmmoCount > ClipSize)
		{
			TotalAmmoCount = TotalAmmoCount - (ClipSize - CurrentAmmoCount);
			CurrentAmmoCount = ClipSize;
		}
		else
		{
			CurrentAmmoCount = CurrentAmmoCount + TotalAmmoCount;
			TotalAmmoCount = 0;
		}
	}
	
	GetWorldTimerManager().ClearTimer(ReloadTimer);
	IsReloading = false;
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

int AWeaponBase::GetCurrentAmmo_Implementation() 
{
	return CurrentAmmoCount;
}

int AWeaponBase::GetTotalAmmoCount_Implementation() 
{
	return TotalAmmoCount;
}

void AWeaponBase::GetWeaponInfo_Implementation() 
{
	
}

void AWeaponBase::UpdateAmmoUMG_Implementation() 
{
	
}

void AWeaponBase::GetTraceParams_Implementation(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy) 
{
	TraceLocation = in_Location;
	TraceRotation = in_Rotation;
	TraceActorToIgnore = ActorToIgnore;
	AccuracyMultiplier = in_Accuracy;
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

//Remove
void AWeaponBase::GetGunImpulse_Implementation(float out_GunImpulse, float out_HeadMultiplier) 
{
	out_GunImpulse = GunImpulse; 
	out_HeadMultiplier = HeadMultiplier;
}

void AWeaponBase::SetWeaponAmmo_Implementation(const int32 in_CurrentAmmo, const int32 in_TotalAmmoCount) 
{
	CurrentAmmoCount = in_CurrentAmmo;
	TotalAmmoCount = in_TotalAmmoCount;
}

void AWeaponBase::EnableWeaponDebug_Implementation(bool DebugStatus) 
{
	bDebuggingMode = DebugStatus;
}

bool AWeaponBase::IsInAutoMode_Implementation() 
{
	return InAutoMode;
}

void AWeaponBase::FadeInUMG(float Alpha) 
{
	AmmoWidgetComponent->SetTintColorAndOpacity(FLinearColor(1,1,1,Alpha));
}

void AWeaponBase::MoveUMG(bool bRightShoulder) 
{
	if(bRightShoulder)
	{
		AmmoWidgetComponent->SetRelativeLocation(UMG_RightLocation->GetComponentLocation());
	}
	else
	{
		AmmoWidgetComponent->SetRelativeLocation(UMG_LeftLocation->GetComponentLocation());
	}
}

bool AWeaponBase::LineTrace(FHitResult& Hit, FVector& ShotDirection) 
{
	FVector ForwardVector = (FRotationMatrix(TraceRotation).GetScaledAxis( EAxis::X ));
	FVector NewBulletSpread;
	CalculateBulletSpread(NewBulletSpread);
	ShotDirection = -TraceRotation.Vector();
	FVector LineEnd = ((ForwardVector * GunRange) + TraceLocation) + NewBulletSpread;
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

void AWeaponBase::Shoot() 
{
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerShoot();
	}
	if(CanShoot())
	{
		CurrentAmmoCount--;
		if(bDebuggingMode){CurrentAmmoCount++;}
		FHitResult Hit;
		FVector ShotDirection;
		bool bSuccess = LineTrace(Hit, ShotDirection);
		//Defaults for Impact effects
		FVector TracerEndPoint = Hit.TraceEnd;
		EPhysicalSurface SurfaceType = SurfaceType_Default;
		if(bSuccess)
		{
			if(HasAuthority())	{UE_LOG(LogTemp,Warning,TEXT("ServerShoot"));}
			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
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
				//DrawDebugPoint(GetWorld(), BlindFireHit.Location, 20, FColor::Red, true);
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

				}
				else
				{
					//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Blue, true);
				}
			}
			TracerEndPoint = Hit.ImpactPoint;
			PlayImpactEffects(SurfaceType, TracerEndPoint);
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
	Shoot();
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

void AWeaponBase::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint) 
{
	UParticleSystem* SelectedParticle = nullptr; 
	switch(SurfaceType)
		{
		case SurfaceType1:
		case SurfaceType2:
			SelectedParticle = DefaultImpactParticles;
			break;
		case SurfaceType3:
			SelectedParticle = DefaultImpactParticles;
			break;
		case SurfaceType4:
			SelectedParticle = DefaultImpactParticles;
			break;
		default:
			SelectedParticle = DefaultImpactParticles;
			break;
		}
	if(SelectedParticle)
	{
		FVector MuzzleLocation = Muzzle->GetComponentLocation();
		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedParticle, ImpactPoint, ShotDirection.Rotation());
	}

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
	if(IsReloading){return false;}
	if(CurrentAmmoCount <= 0){UE_LOG(LogTemp, Warning, TEXT("CurrentAmmo 0 (Weapon)")); return false;}
	else{return true;}
}


void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 	DOREPLIFETIME_CONDITION(AWeaponBase, HitScanTrace, COND_SkipOwner);
	DOREPLIFETIME(AWeaponBase, TraceLocation);
	DOREPLIFETIME(AWeaponBase, CurrentAmmoCount);

}
