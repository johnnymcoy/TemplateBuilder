// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSCustomCharacter.h"

#include "AIController.h"
#include "TemplateBuilder/Interactable/InteractableInterface.h"
#include "TemplateBuilder/Interactable/WeaponPickupBase.h"
#include "Character/Animation/ALSCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "TemplateBuilder/Health/HealthComponent.h"
#include "TemplateBuilder/Weapons/WeaponBase.h"
#include "TemplateBuilder/Components/ShootingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ALSCustomController.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "TemplateBuilder/DamageTypes/ShieldRegen.h"

//TODO: Picking up gun always destroys actor

AALSCustomCharacter::AALSCustomCharacter(const FObjectInitializer& ObjectInitializer)
: AALSCharacter (ObjectInitializer)
{
	Gun = CreateDefaultSubobject<UChildActorComponent>(TEXT("GUN"));
	Gun->SetupAttachment(HeldObjectRoot);
	Gun->SetChildActorClass(AWeaponBase::StaticClass());
	Gun->SetIsReplicated(true);
	
	ShootingComponent = CreateDefaultSubobject<UShootingComponent>("ShootingComponent");
	
	ThrowPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ThrowPoint"));
	ThrowPoint->SetupAttachment(GetMesh(), TEXT("head"));
	
	PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimation"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &AALSCustomCharacter::OnHealthChanged);
	HealthComponent->bHasShield = true;
	OnTakeAnyDamage.AddDynamic(this, &AALSCustomCharacter::AnyDamageTaken);
	SetupPhysicalAnimationDefaults();
	//Defaults in case nothing is set
	PickupThrowIntensity = 500;
	ShieldTimeToRegen = 5.0f;
}

void AALSCustomCharacter::BeginPlay() 
{
	Super::BeginPlay();
	SetupPhysicalAnimation();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AALSCustomCharacter::OnOverlapBegin);
	SkeletalMesh->SetHiddenInGame(true);
	DefaultAccuracy = Accuracy;

	//ShootingComponent setters
	ShootingComponent->SetIsNPC(bIsNPC);
	ShootingComponent->SetGunChildActor(Gun);
	ShootingComponent->SetThrowPoint(ThrowPoint);
	ShootingComponent->SetOwnerMesh(GetMesh());
	ShootingComponent->SetAnimInstance(MainAnimInstance);
	if(!bIsNPC)
	{
		ShootingComponent->SetupHUD();
	}
	ShootingComponent->SetController(GetController());
	if(CurrentWeaponData.MeshForPickup == nullptr){bIsHolstered = true;}
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
	PlayerInputComponent->BindAction("ShootAction", IE_Pressed, this, &AALSCustomCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("ShootAction", IE_Released, this, &AALSCustomCharacter::FireButtonReleased);
	PlayerInputComponent->BindAction("AimAction", IE_Pressed, this, &AALSCustomCharacter::AimPressedAction);
	PlayerInputComponent->BindAction("AimAction", IE_Released, this, &AALSCustomCharacter::AimReleasedAction);
	PlayerInputComponent->BindAction("ReloadAction", IE_Pressed, this, &AALSCustomCharacter::ReloadButtonPressed);
	PlayerInputComponent->BindAction("AutoModeAction", IE_Pressed, this, &AALSCustomCharacter::SwitchAutoModePressed);
	PlayerInputComponent->BindAction("SwapWeaponAction", IE_Pressed, this, &AALSCustomCharacter::SwapWeaponPressed);
	PlayerInputComponent->BindAction("SwapWeaponAction", IE_Released, this, &AALSCustomCharacter::SwapWeaponReleased);
	PlayerInputComponent->BindAction("ThrowWeaponAction", IE_Pressed, this, &AALSCustomCharacter::ThrowWeaponPressed);

	PlayerInputComponent->BindAction("CameraAction", IE_Pressed, this, &AALSCustomCharacter::CameraButtonPressed);
}

//////Done
void AALSCustomCharacter::AimPressedAction() 
{
	Super::AimPressedAction();
	bIsAiming = true;
	if(ShootingComponent)
	{
		ShootingComponent->AimPressed(bRightShoulder);
	}
}
//Done
void AALSCustomCharacter::AimReleasedAction() 
{
	Super::AimReleasedAction();
	bIsAiming = false;
	if(ShootingComponent)
	{
		ShootingComponent->AimReleased(bRightShoulder);
	}
}

void AALSCustomCharacter::FireButtonPressed()
{
	if(MantleTimeline->IsPlaying()){return;}
	if(ShootingComponent && !ShootingComponent->GetPlayerShootingStatus().bIsHolstered)
	{
		ShootGun();
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

void AALSCustomCharacter::FireButtonReleased()
{
	if(ShootingComponent)
	{
		ShootingComponent->StopShootGun();
	}
}

void AALSCustomCharacter::ReloadButtonPressed()
{
	if(ShootingComponent)
	{
		UAnimMontage* ReloadAnimation = GetReloadAnimation(ShootingComponent->GetCurrentWeaponData().WeaponType);
		ShootingComponent->Reload(ReloadAnimation);
	}
}

//Done
void AALSCustomCharacter::CameraButtonPressed() 
{
	CameraPressedAction();
	if(ShootingComponent)
	{
		ShootingComponent->MoveUMG(bRightShoulder);
	}
}

//TODO fix throw weapon (check character_BP) 
void AALSCustomCharacter::ThrowWeaponPressed()
{
	if(ShootingComponent)
	{
		CalculateAccuracy();
		ShootingComponent->ThrowWeapon(CurrentWeaponData);
		SetOverlayState(EALSOverlayState::Default);
	}
}


void AALSCustomCharacter::ShootGun() 
{
	if(ShootingComponent)
	{
		CalculateAccuracy();
		ShootingComponent->ValidateShootGun();
	}
}

//Still needed
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
	if(bIsNPC)
	{
		Accuracy = (Accuracy * 0.25f);
	}
	ShootingComponent->SetAccuracy(Accuracy);
}
//Done
void AALSCustomCharacter::SwitchAutoModePressed() 
{
	if(ShootingComponent)
	{
		ShootingComponent->SwitchAutoMode();
	}
}

// timer function Swap weapon
void AALSCustomCharacter::SwapWeaponPressed()
{
	if(ShootingComponent)
	{
		ShootingComponent->SwapWeaponPressed();
		if(ShootingComponent->GetPlayerShootingStatus().bIsHolstered)
		{
			SetOverlayState(EALSOverlayState::Default);
		}
		else
		{
			SetOverlayState(ShootingComponent->GetCurrentWeaponData().OverlayState);
		}
	}
}

void AALSCustomCharacter::SwapWeaponReleased()
{
	if(ShootingComponent)
	{
		ShootingComponent->SwapWeaponReleased();
		if(ShootingComponent->GetPlayerShootingStatus().bIsHolstered)
		{
			SetOverlayState(EALSOverlayState::Default);
		}
		else
		{
			SetOverlayState(ShootingComponent->GetCurrentWeaponData().OverlayState);
		}
	}
}

void AALSCustomCharacter::PickupGunEvent_Implementation(const FWeaponData in_WeaponData) 
{
	if(ShootingComponent)
	{
		ShootingComponent->PickupWeapon(in_WeaponData);
		SetOverlayState(ShootingComponent->GetCurrentWeaponData().OverlayState);
		// ServerSetOverlayState(ShootingComponent->GetCurrentWeaponData().OverlayState);
	}
}

void AALSCustomCharacter::DestroyActor_Implementation(AActor* ActorToDestroy)
{
	DestroyActorOnClient(ActorToDestroy);
}

void AALSCustomCharacter::DestroyActorOnClient_Implementation(AActor* ActorToDestroy)
{
	if(ActorToDestroy != nullptr)
	{
		ActorToDestroy->Destroy();
	}
}


void AALSCustomCharacter::ThrowWeaponEvent_Implementation(FWeaponData WeaponData) 
{
	if(ShootingComponent)
	{
		CalculateAccuracy();
		ShootingComponent->ThrowWeapon(WeaponData);
		SetOverlayState(EALSOverlayState::Default);
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
	if(ShootingComponent)
	{
		ShootingComponent->Death();
	}
	// if(GetLocalRole() < ROLE_Authority)
	// {
	// 	ServerClearWeapon();
	// }
	//todo Shooting stuff
	SetReplicatingMovement(true);
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

	////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
	//////////////////////////		HEALTH		HEALTH		HEALTH		  //////////////////////////
	////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////

void AALSCustomCharacter::AnyDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) 
{
	GetWorldTimerManager().SetTimer(HealthTimer, this, &AALSCustomCharacter::HealthDelay, 0.001f, false);

	GetWorldTimerManager().ClearTimer(ShieldTimeToRegenHandle);
	GetWorldTimerManager().SetTimer(ShieldTimeToRegenHandle, this, &AALSCustomCharacter::ShieldRegen, 0.01f, true, ShieldTimeToRegen);
	
	//GetWorldTimerManager
	//Clear Old Timer in case more damager
	//Set Timer
	//heal sheild function,
	//Set timer 0.1 + 1 shield
}

void AALSCustomCharacter::ShieldRegen()
{
	if(HealthComponent->ShieldHealth < HealthComponent->DefaultShieldHealth)
	{
		HealthComponent->ShieldRegen();
		UE_LOG(LogTemp,Warning,TEXT("Heal"));
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ShieldTimeToRegenHandle);
	}
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

//
//Character Pushing
// V0.4 Not completely working, can levitate player, screws up feet, etc.
void AALSCustomCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	// UE_LOG(LogTemp, Warning, TEXT("HIT"));
	// //Needs something to prevent from casting 10000 times per second
	// //Check Speed? + Inpulse? 
	// ACharacter* OtherCharacter = Cast<ACharacter>(Other);
	// if(OtherCharacter)
	// {
	// 	//Get Opposite Direction
	// 	FRotator RotationX = UKismetMathLibrary::MakeRotFromX(HitNormal);
	// 	FRotator RotatorFlip = UKismetMathLibrary::MakeRotator(0.0f,0.0f,(RotationX.Yaw - 180.0f));
	// 	FVector ForwardVector = (FRotationMatrix(RotatorFlip).GetScaledAxis(EAxis::X));
	// 	//Speed 
	// 	float ClampedFloat = FMath::Clamp(GetVelocity().Size(), 50.0f, 500.0f);
	// 	FVector2D RangeA = {50.0f, 500.0f};
	// 	FVector2D RangeB = {50.0f, 130.0f};
	// 	float ClampedRange = FMath::GetMappedRangeValueClamped(RangeA,RangeB, ClampedFloat);
	// 	
	// 	float ClampedSmall = FMath::Clamp((ClampedRange * 0.45f), 0.0f, 10.0f);
	// 	FVector AddedVector = UKismetMathLibrary::MakeVector(0,0,ClampedSmall);
	// 	FVector LaunchVelocity = (ForwardVector * ClampedRange) + AddedVector;
	// 	OtherCharacter->LaunchCharacter(LaunchVelocity, false, false);
	// }
}

bool AALSCustomCharacter::IsDead() const
{
	if(HealthComponent->Health > 0) 
	{
		return false;
	}
	return true;
}

bool AALSCustomCharacter::IsCrouching() const
{
	return Stance == EALSStance::Crouching;
}

void AALSCustomCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AALSCustomCharacter, DeathOnce);
	DOREPLIFETIME_CONDITION(AALSCustomCharacter, CurrentWeaponData, COND_SkipOwner);
}


