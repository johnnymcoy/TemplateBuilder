// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomPlayerCharacter.h"
#include "Components/CharacterHealthComponent.h"
#include "Components/DialogueComponent.h"
#include "Components/InteractionComponent.h"
#include "Character/Animation/ALSCharacterAnimInstance.h"



ACustomPlayerCharacter::ACustomPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: ACustomCharacterBase(ObjectInitializer)
{
	CharacterHealthComponent->SetHasShield(true, 100);
	DialogueComponent->InitialSetup();
	InteractionComponent->SetupComponent(SkeletalMesh, MainAnimInstance, Controller, bIsNPC, bIsDead);
}

void ACustomPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// PlayerInputComponent->BindAction("UseAction", IE_Pressed, this, &ACustomPlayerCharacter::Use);
	PlayerInputComponent->BindAction("AimAction", IE_Pressed, this, &ACustomPlayerCharacter::AimPressedAction);
	PlayerInputComponent->BindAction("AimAction", IE_Released, this, &ACustomPlayerCharacter::AimReleasedAction);

}

void ACustomPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACustomPlayerCharacter::AimPressedAction()
{
	Super::AimPressedAction();
	InteractionComponent->StartTraceForward();
	
}

void ACustomPlayerCharacter::AimReleasedAction()
{
	Super::AimReleasedAction();
	InteractionComponent->StopTraceForward();

	
}
