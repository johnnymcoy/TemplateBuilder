// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomCharacterBase.h"

#include "Components/CharacterShootingComponent.h"
#include "Components/CharacterHealthComponent.h"
#include "Components/DialogueComponent.h"
#include "Character/Animation/ALSCharacterAnimInstance.h"
#include "Components/InteractionComponent.h"

DEFINE_LOG_CATEGORY(LogCustomCharacters);


ACustomCharacterBase::ACustomCharacterBase(const FObjectInitializer& ObjectInitializer)
	: AALSCharacter(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Health
	CharacterHealthComponent = CreateDefaultSubobject<UCharacterHealthComponent>(TEXT("Character Health"));
	CharacterHealthComponent->SetupComponent(SkeletalMesh, nullptr, Controller, bIsNPC, bIsDead);
	CharacterHealthComponent->OnHealthChanged.AddDynamic(this, &ACustomCharacterBase::OnHealthChanged);
	CharacterHealthComponent->OnHealthAndShieldChanged.AddDynamic(this, &ACustomCharacterBase::OnHealthAndShieldChanged);
	CharacterHealthComponent->OnInjuredBodyPart.AddDynamic(this, &ACustomCharacterBase::OnInjuredBodyPart);
	CharacterHealthComponent->OnShieldBreak.AddDynamic(this, &ACustomCharacterBase::OnShieldBreak);
	CharacterHealthComponent->OnDeath.AddDynamic(this, &ACustomCharacterBase::OnDeath);

	// Dialogue
	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("Dialogue"));
	DialogueComponent->SetupComponent(nullptr, nullptr, Controller, bIsNPC, bIsDead);

	// Shooting
	ShootingComponent = CreateDefaultSubobject<UCharacterShootingComponent>("Shooting");
	ShootingComponent->OnWeaponEqiupped.AddDynamic(this, &ACustomCharacterBase::WeaponEquipped);
	ShootingComponent->OnWeaponStateChanged.AddDynamic(this, &ACustomCharacterBase::WeaponStateChanged);
	// ShootingComponent->SetupComponent(SkeletalMesh, MainAnimInstance, Controller, bIsNPC, bIsDead);

	// Interaction
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("Interaction");
	// InteractionComponent->SetupComponent(SkeletalMesh, MainAnimInstance, Controller, bIsNPC, bIsDead);


	// Deafults for some of the ALS character - from the BP 
	bRightShoulder = true;
	GroundedTraceSettings.MaxLedgeHeight = 250.0f;
	GroundedTraceSettings.MinLedgeHeight = 50.0f;
	GroundedTraceSettings.ReachDistance = 75.0f;
	GroundedTraceSettings.ForwardTraceRadius = 30.0f;
	GroundedTraceSettings.DownwardTraceRadius = 30.0f;

	AutomaticTraceSettings.MaxLedgeHeight = 80.0f;
	AutomaticTraceSettings.MinLedgeHeight = 40.0f;
	AutomaticTraceSettings.ReachDistance = 50.0f;
	AutomaticTraceSettings.ForwardTraceRadius = 30.0f;
	AutomaticTraceSettings.DownwardTraceRadius = 30.0f;

	FallingTraceSettings.MaxLedgeHeight = 150.0f;
	FallingTraceSettings.MinLedgeHeight = 50.0f;
	FallingTraceSettings.ReachDistance = 70.0f;
	FallingTraceSettings.ForwardTraceRadius = 30.0f;
	FallingTraceSettings.DownwardTraceRadius = 30.0f;
	bRagdollOnLand = true;
	SkeletalMesh->SetHiddenInGame(true);
	
	
}

void ACustomCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if(ShootingComponent != nullptr)
	{
		ShootingComponent->SetupComponent(GetMesh(), MainAnimInstance, Controller, bIsNPC, bIsDead);
	}
	if(InteractionComponent != nullptr)
	{
		InteractionComponent->SetupComponent(GetMesh(), MainAnimInstance, Controller, bIsNPC, bIsDead);
	}

	
}

void ACustomCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(ShootingComponent != nullptr)
	{
		ShootingComponent->SetupPlayerInputComponent(PlayerInputComponent);
	}
	PlayerInputComponent->BindAction("ShootAction", IE_Pressed, this, &ACustomCharacterBase::FirePressedAction);
	// PlayerInputComponent->BindAction("ShootAction", IE_Released, this, &ACustomCharacterBase::FireReleasedAction);
	// PlayerInputComponent->BindAction("AimAction", IE_Pressed, this, &ACustomCharacterBase::AimPressedAction);
	// PlayerInputComponent->BindAction("AimAction", IE_Released, this, &ACustomCharacterBase::AimReleasedAction);
	// PlayerInputComponent->BindAction("ReloadAction", IE_Pressed, this, &ACustomCharacterBase::ReloadPressedAction);
	// PlayerInputComponent->BindAction("AutoModeAction", IE_Pressed, this, &ACustomCharacterBase::SwitchAutoModePressedAction);
	// PlayerInputComponent->BindAction("SwapWeaponAction", IE_Pressed, this, &ACustomCharacterBase::SwapWeaponPressedAction);
	// PlayerInputComponent->BindAction("SwapWeaponAction", IE_Released, this, &ACustomCharacterBase::SwapWeaponReleasedAction);
	// PlayerInputComponent->BindAction("CameraAction", IE_Pressed, this, &AALSCustomCharacter::CameraButtonPressed);

}

void ACustomCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////				Inputs						  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////


// void ACustomCharacterBase::AimPressedAction()
// {
// 	Super::AimPressedAction();
// 	if(ShootingComponent != nullptr)
// 	{
// 		ShootingComponent->AimPressed(bRightShoulder);
// 	}
// }

// void ACustomCharacterBase::AimReleasedAction()
// {
// 	Super::AimReleasedAction();
// 	if(ShootingComponent != nullptr)
// 	{
// 		ShootingComponent->AimReleased(bRightShoulder);
// 	}	
// }

void ACustomCharacterBase::FirePressedAction()
{
	if(MantleTimeline->IsPlaying()){return;}
	if(ShootingComponent != nullptr)
	{
		if(!ShootingComponent->GetPlayerWeaponState().bIsHolstered)
		{
			ShootingComponent->ShootGun();
			//! ShootGun();
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
}

// ! void ACustomCharacterBase::FireReleasedAction()
// {
// 	if(ShootingComponent != nullptr)
// 	{
// 		ShootingComponent->StopShootGun();
// 	}
// }

// void ACustomCharacterBase::ReloadPressedAction()
// {
// 	if(ShootingComponent != nullptr)
// 	{
// 		// UAnimMontage* ReloadAnimation = GetReloadAnimation(ShootingComponent->GetCurrentWeaponData().WeaponType);
// 		ShootingComponent->Reload();
// 	}
// }

// void ACustomCharacterBase::SwapWeaponPressedAction()
// {
// 	if(ShootingComponent != nullptr)
// 	{
// 		
// 	}
// }

// void ACustomCharacterBase::SwapWeaponReleasedAction()
// {
// 	
// }
//
// void ACustomCharacterBase::SwitchAutoModePressedAction()
// {
// 	
// }
//
// void ACustomCharacterBase::ThrowWeaponPressedAction()
// {
// 	
// }






////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////				Character Interface			  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////

void ACustomCharacterBase::GetDialogueComponent(UDialogueComponent*& Out_DialogueComponent) const
{
	Out_DialogueComponent = DialogueComponent;
}

void ACustomCharacterBase::PickupGunEvent(const FWeaponData_T In_WeaponData)
{
	bool bWeaponWeHave;
	ShootingComponent->PickupWeapon(In_WeaponData, bWeaponWeHave);
	if(!bWeaponWeHave)
	{
		const EALSOverlayState WeaponOverlayState = WeaponStateToOverlayState(In_WeaponData.WeaponOverlay);
		SetOverlayState(WeaponOverlayState);
	}
}

//////////////////////////- |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////-				Shooting Component Bind		  //////////////////////////
//////////////////////////- |||||||||||||||||||||||||||||||||||||||||| //////////////////////////

void ACustomCharacterBase::WeaponEquipped(TArray<FWeaponData_T> Weapons, float CurrentWeaponIndex)
{
	UE_LOG(LogCustomCharacters, Warning, TEXT("Weapon Equipped Custom Character"));

	if(Weapons.IsValidIndex(CurrentWeaponIndex))
	{
		const EALSOverlayState WeaponOverlayState = WeaponStateToOverlayState(Weapons[CurrentWeaponIndex].WeaponOverlay);
		SetOverlayState(WeaponOverlayState);
	}
}

void ACustomCharacterBase::WeaponStateChanged(FPlayerWeaponState PlayerWeaponState)
{
	UE_LOG(LogCustomCharacters, Warning, TEXT("Weapon State Changed Custom Character"));
	if(PlayerWeaponState.bIsHolstered)
	{
		SetOverlayState(EALSOverlayState::Default);
	}
}


//- Helper Function // 
EALSOverlayState ACustomCharacterBase::WeaponStateToOverlayState(EWeaponOverlay WeaponOverlay)
{
	switch (WeaponOverlay)
	{
	case EWeaponOverlay::PistolOneHanded:
		return EALSOverlayState::PistolOneHanded;
	case EWeaponOverlay::PistolTwoHanded:
		return EALSOverlayState::PistolTwoHanded;
	case EWeaponOverlay::Shotgun:
		return EALSOverlayState::Shotgun;
	case EWeaponOverlay::Rifle:
		return EALSOverlayState::Rifle;
	default:
		UE_LOG(LogTemp,Warning,TEXT("No Weapon Overlay Set"));
		return EALSOverlayState::PistolOneHanded;
	}
}



//////////////////////////- |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////-				Health						  //////////////////////////
//////////////////////////- |||||||||||||||||||||||||||||||||||||||||| //////////////////////////

void ACustomCharacterBase::OnHealthChanged(UHealthComponentBase* HealthComponent, float Health, float MaxHealth,
                                           const UDamageType* DamageType)
{
	
}

void ACustomCharacterBase::OnHealthAndShieldChanged(UHealthComponentBase* HealthComponent, float Health,
	float MaxHealth, float ShieldHealth, float MaxShieldHealth, const UDamageType* DamageType)
{
	
}

void ACustomCharacterBase::OnInjuredBodyPart(FName InjuredBodyPart)
{
}

void ACustomCharacterBase::OnShieldBreak()
{
}

void ACustomCharacterBase::OnDeath(AActor* OwningActor)
{
	bIsDead = true;
}


////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
//////////////////////////				Pure Functions				  //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////

bool ACustomCharacterBase::IsDead() const
{
	return bIsDead;
}

bool ACustomCharacterBase::IsCrouching() const
{
	return Stance == EALSStance::Crouching;
}

////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////
////////////////////////// |||||||||||||||||||||||||||||||||||||||||| //////////////////////////


