// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponFramework.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AWeaponFramework::AWeaponFramework()
{
 	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	RootComponent = GunMeshComponent;
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	Muzzle->SetupAttachment(GunMeshComponent);
	
}

// Called when the game starts or when spawned
void AWeaponFramework::BeginPlay()
{
	Super::BeginPlay();
	if(WeaponData.bHasAutoMode)	{WeaponData.bIsInAutoMode = true;}
	
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
		// WeaponEffects->PlayDryFire();
		// UGameplayStatics::PlaySoundAtLocation(this, DryFireSound, GetActorLocation());
		// PlayDryFire();
	}
}
// Formally Shoot();
void AWeaponFramework::ServerShoot_Implementation()
{
	if(CanShoot())
	{
		//TODO add debugging
		// if(bDebuggingMode){GunWeaponData.CurrentAmmo++;}
		WeaponData.CurrentAmmo--;
		
		FHitResult Hit;
		FVector ShotDirection;
		bool bSuccess = LineTrace(Hit, ShotDirection);

		// Defaults for Impact Effects
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
				// I.E gun against the wall 
				BlindFireWeapon();
			}
			else
			{
				//Check if Anything is between the player and where they should be shooting //
				//i.e. can see someone around the corner but gun is pointed at the wall //
				FHitResult HitCheck;
				ActorsToIgnore.Add(Hit.GetActor());

				// todo Debugging
				EDrawDebugTrace::Type DrawDebugType;
				DrawDebugType = EDrawDebugTrace::ForDuration;
				// if(bDebuggingMode){DrawDebugType = EDrawDebugTrace::ForDuration;}else{DrawDebugType = EDrawDebugTrace::None;}

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
						// APawn* OwnerPawn = Cast<APawn>(GetOwner());
						// ReceiveFire(Hit, OwnerPawn);
						// ApplyDamageToActor(Hit, ShotDirection);
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
}

bool AWeaponFramework::ServerShoot_Validate()
{
	return true;
}

bool AWeaponFramework::LineTrace(FHitResult& Hit, FVector& ShotDirection) 
{
	const FVector ForwardVector = (FRotationMatrix(TraceRotation).GetScaledAxis( EAxis::X ));
	FVector NewBulletSpread;
	// CalculateBulletSpread(NewBulletSpread);
	// ShotDirection = -TraceRotation.Vector();
	const FVector LineEnd = ((ForwardVector * WeaponStats.GunRange) + TraceLocation) + NewBulletSpread;

	// todo Debugging
	EDrawDebugTrace::Type DrawDebugType;
	DrawDebugType = EDrawDebugTrace::ForDuration;
	// if(bDebuggingMode){DrawDebugType = EDrawDebugTrace::ForDuration;}else{DrawDebugType = EDrawDebugTrace::None;}

	return UKismetSystemLibrary::LineTraceSingle(this, TraceLocation, LineEnd, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
	true, ActorsToIgnore, DrawDebugType, Hit, true, FLinearColor::Red, FLinearColor::Red, 1.0f);
}


void AWeaponFramework::Reload(float ReloadTime)
{
	
}

void AWeaponFramework::CancelReload()
{
	
}

void AWeaponFramework::SwitchAutoMode()
{
	
}

void AWeaponFramework::BlindFireWeapon()
{
	
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

