// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomAICharacter.h"

#include "Components/CompanionComponent.h"
#include "Components/CompanionMasterComponent.h"
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
	if(CompanionComponent != nullptr)
	{
		CompanionComponent->SetupComponent(GetMesh(), MainAnimInstance, Controller, bIsNPC, bIsDead);
		CompanionComponent->SetupBlackboard();
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
	if(CompanionComponent != nullptr)
	{
		CompanionComponent->SetMasterActor(Caller);	
		// if(Caller->GetComponentByClass(UCompanionMasterComponent::StaticClass()))
		// {
		// }
		CompanionComponent->Follow();
	}
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

