// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomAICharacter.h"

ACustomAICharacter::ACustomAICharacter(const FObjectInitializer& ObjectInitializer)
	: ACustomCharacterBase(ObjectInitializer)
{
	
}

void ACustomAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACustomAICharacter::BeginPlay()
{
	Super::BeginPlay();
}
