// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomPlayerCharacter.h"
#include "Components/CharacterHealthComponent.h"
#include "Components/DialogueComponent.h"
#include "Components/InteractionComponent.h"
#include "Character/Animation/ALSCharacterAnimInstance.h"
#include "Components/CharacterShootingComponent.h"


ACustomPlayerCharacter::ACustomPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: ACustomCharacterBase(ObjectInitializer)
{
	CharacterHealthComponent->SetHasShield(true, 100);
}

void ACustomPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("UseAction", IE_Pressed, this, &ACustomPlayerCharacter::UseAction);
	PlayerInputComponent->BindAction("AimAction", IE_Pressed, this, &ACustomPlayerCharacter::AimPressedAction);
	PlayerInputComponent->BindAction("AimAction", IE_Released, this, &ACustomPlayerCharacter::AimReleasedAction);

}

void ACustomPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(DialogueComponent != nullptr)
	{
		DialogueComponent->SetupComponent(SkeletalMesh, MainAnimInstance, Controller, bIsNPC, bIsDead);
		DialogueComponent->InitialSetup();
	}
	if(InteractionComponent != nullptr)
	{
		InteractionComponent->SetupComponent(SkeletalMesh, MainAnimInstance, Controller, bIsNPC, bIsDead);
	}
}

void ACustomPlayerCharacter::AimPressedAction()
{
	Super::AimPressedAction();
	if(InteractionComponent != nullptr)
	{
		InteractionComponent->StartTraceForward();
	}
	// if(ShootingComponent != nullptr)
	// {
	// 	ShootingComponent->AimPressed(bRightShoulder);
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

void ACustomPlayerCharacter::UseAction()
{
	InteractionComponent->Use();
	// if(GetLocalRole() < ROLE_Authority)
	// {
	// 	ServerUse();
	// }
	// if(HasAuthority())
	// {
	// 	// UE_LOG(LogTemp,Warning, TEXT("Server USE"));
	// }
}
