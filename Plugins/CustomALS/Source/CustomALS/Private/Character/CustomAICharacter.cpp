// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomAICharacter.h"

#include "Components/CompanionComponent.h"
#include "Character/Animation/ALSCharacterAnimInstance.h"

ACustomAICharacter::ACustomAICharacter(const FObjectInitializer& ObjectInitializer)
	: ACustomCharacterBase(ObjectInitializer)
{
	bIsNPC = true;
	CompanionComponent = CreateDefaultSubobject<UCompanionComponent>("CompanionComponent");
}

void ACustomAICharacter::BeginPlay()
{
	Super::BeginPlay();
	// SetupComponents();
}

void ACustomAICharacter::SetupComponents()
{
	Super::SetupComponents();
	if(CompanionComponent != nullptr)
	{
		CompanionComponent->SetupComponent(GetMesh(), MainAnimInstance, Controller, bIsNPC, bIsDead);
		CompanionComponent->SetupBlackboard();
		CompanionComponent->DisplayName = DisplayName;
	}
}

void ACustomAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACustomAICharacter::OnInteract(AActor* Caller)
{
	Super::OnInteract(Caller);
}

void ACustomAICharacter::OnPickUp(AActor* Caller)
{
	Super::OnPickUp(Caller);
}

void ACustomAICharacter::StartFocus()
{
	Super::StartFocus();
}

void ACustomAICharacter::EndFocus()
{
	Super::EndFocus();
}

