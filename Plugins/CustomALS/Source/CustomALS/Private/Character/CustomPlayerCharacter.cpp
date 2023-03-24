// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomPlayerCharacter.h"
#include "Components/CharacterHealthComponent.h"
#include "Components/DialogueComponent.h"
#include "Components/InteractionComponent.h"
#include "Character/Animation/ALSCharacterAnimInstance.h"
#include "Components/CharacterShootingComponent.h"
#include "Components/CompanionMasterComponent.h"
#include "Components/PlayerCharacterShootingComponent.h"
#include "Interfaces/AICharacter.h"
#include "Interfaces/CompanionInterface.h"


ACustomPlayerCharacter::ACustomPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: ACustomCharacterBase(ObjectInitializer)
{
	CharacterHealthComponent->SetHasShield(true, 100);
	// ShootingComponent = CreateDefaultSubobject<UPlayerCharacterShootingComponent>("PlayerShootingComponent");

	PlayerShootingComponent = CreateDefaultSubobject<UPlayerCharacterShootingComponent>("PlayerShootingComponent");
	PlayerShootingComponent->OnWeaponEquipped.AddDynamic(this, &ACustomCharacterBase::WeaponEquipped);
	PlayerShootingComponent->OnWeaponStateChanged.AddDynamic(this, &ACustomCharacterBase::WeaponStateChanged);
	PlayerShootingComponent->SetThrowPoint(ThrowPoint);

	CompanionMasterComponentComponent = CreateDefaultSubobject<UCompanionMasterComponent>("CompanionMasterComponent");
}

void ACustomPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(PlayerShootingComponent != nullptr)
	{
		PlayerShootingComponent->SetupComponent(GetMesh(), MainAnimInstance, Controller, bIsNPC, bIsDead);
		if(IsLocallyControlled())
		{
			PlayerShootingComponent->SetupWeaponWidget();
			PlayerShootingComponent->SetupWeaponCrosshairWidget();
		}
	}
	if(CompanionMasterComponentComponent != nullptr)
	{
		CompanionMasterComponentComponent->SetupComponent(GetMesh(), MainAnimInstance, Controller, bIsNPC, bIsDead);
		if(IsLocallyControlled())
		{
			CompanionMasterComponentComponent->SetupCompanionWidget();
		}
	}
	if(DialogueComponent != nullptr)
	{
		DialogueComponent->SetupComponent(GetMesh(), MainAnimInstance, Controller, bIsNPC, bIsDead);
		DialogueComponent->InitialSetup();
	}
	if(InteractionComponent != nullptr)
	{
		InteractionComponent->SetupComponent(GetMesh(), MainAnimInstance, Controller, bIsNPC, bIsDead);
	}
}

void ACustomPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(PlayerShootingComponent != nullptr)
	{
		PlayerShootingComponent->SetupPlayerInputComponent(PlayerInputComponent);
	}
	PlayerInputComponent->BindAction("UseAction", IE_Pressed, this, &ACustomPlayerCharacter::UseAction);
	PlayerInputComponent->BindAction("AimAction", IE_Pressed, this, &ACustomPlayerCharacter::AimPressedAction);
	PlayerInputComponent->BindAction("AimAction", IE_Released, this, &ACustomPlayerCharacter::AimReleasedAction);
}


void ACustomPlayerCharacter::AimPressedAction()
{
	Super::AimPressedAction();
	if(InteractionComponent != nullptr)
	{
		InteractionComponent->StartTraceForward();
	}
	// if(PlayerShootingComponent != nullptr)
	// {
	// 	PlayerShootingComponent->AimPressed(bRightShoulder);
	// }
}

void ACustomPlayerCharacter::AimReleasedAction()
{
	Super::AimReleasedAction();
	if(InteractionComponent != nullptr)
	{
		InteractionComponent->StopTraceForward();
	}
	// if(ShootingComponent != nullptr)
	// {
	// 	ShootingComponent->AimReleased(bRightShoulder);
	// }
}

void ACustomPlayerCharacter::FirePressedAction()
{
	Super::FirePressedAction();
	if(PlayerShootingComponent != nullptr)
	{
		if(!PlayerShootingComponent->GetPlayerWeaponState().bIsHolstered)
		{
			PlayerShootingComponent->ShootGun();
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

void ACustomPlayerCharacter::WeaponEquipped(TArray<FWeaponData_T> Weapons, int32 CurrentWeaponIndex)
{
	Super::WeaponEquipped(Weapons, CurrentWeaponIndex);
	if(PlayerShootingComponent->GetPlayerWeaponState().bIsHolstered){return;}
	if(Weapons.IsValidIndex(CurrentWeaponIndex))
	{
		const EALSOverlayState WeaponOverlayState = WeaponStateToOverlayState(Weapons[CurrentWeaponIndex].WeaponOverlay);
		if(!HasAuthority())
		{
			
		}
		Server_SetOverlayState(WeaponOverlayState);
		//? No owning connection (Server changed weapons, other clients can't do server function)
		SetOverlayState(WeaponOverlayState);
	}
}

int32 ACustomPlayerCharacter::PickupGunEvent(const FWeaponData_T In_WeaponData)
{
	// Super::PickupGunEvent(In_WeaponData);
	// Todo could remove WeaponWeHave and just have int32 Remaining
	int32 RemainingAmmo = 0;
	PlayerShootingComponent->PickupWeapon(In_WeaponData, RemainingAmmo);
	// if(bWeaponWeHave)
	// {
	// 	return RemainingAmmo;
	// }
	return RemainingAmmo;
	return 0;
	
	// if(PlayerShootingComponent->GetPlayerWeaponState().bIsHolstered){return;}
	// if(!bWeaponWeHave)
	// {
	// 	const EALSOverlayState WeaponOverlayState = WeaponStateToOverlayState(In_WeaponData.WeaponOverlay);
	// 	SetOverlayState(WeaponOverlayState);
	// }
}

void ACustomPlayerCharacter::UseAction()
{
	if(InteractionComponent == nullptr){UE_LOG(LogTemp,Error,TEXT("Missong Interaction Component"));return;}
	if(CompanionMasterComponentComponent == nullptr){UE_LOG(LogTemp,Error,TEXT("Missong Interaction Component"));return;}

	AActor* InteractionActor = InteractionComponent->Use();
	
	if(!CompanionMasterComponentComponent->GetIsCommandingCompanion())
	{
		if(InteractionActor != nullptr)
		{
			const IAICharacter* AICharacter = Cast<IAICharacter>(InteractionActor);
			if(AICharacter != nullptr)
			{
				//- open Context Menu, and ask player what i'd like to do //
				// AICharacter->Interact();
				//- If i have clicked Command // 
				CompanionMasterComponentComponent->SetCompanionToCommand(InteractionActor);
			}
		}
	}
	else
	{
		//? Temp, will need multiple context menus 
		FHitResult Hit = InteractionComponent->GetHitResult();
		CompanionMasterComponentComponent->CommandCompanion();
		
	}

	
	
	
	// if(GetLocalRole() < ROLE_Authority)
	// {
	// 	ServerUse();
	// }
	// if(HasAuthority())
	// {
	// 	// UE_LOG(LogTemp,Warning, TEXT("Server USE"));
	// }
}
