// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomPlayerCharacter.h"
#include "Components/CharacterHealthComponent.h"
#include "Components/DialogueComponent.h"


ACustomPlayerCharacter::ACustomPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: ACustomCharacterBase(ObjectInitializer)
{
	CharacterHealthComponent->SetHasShield(true, 100);
	DialogueComponent->InitialSetup();
}

void ACustomPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACustomPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}
