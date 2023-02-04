// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomCharacterBase.h"

#include "Components/CharacterShootingComponent.h"
#include "Components/CharacterHealthComponent.h"
#include "Components/DialogueComponent.h"
#include "Character/Animation/ALSCharacterAnimInstance.h"
#include "Components/InteractionComponent.h"

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
	ShootingComponent->SetupComponent(SkeletalMesh, MainAnimInstance, Controller, bIsNPC, bIsDead);

	// Interaction
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("Interaction");
	InteractionComponent->SetupComponent(SkeletalMesh, MainAnimInstance, Controller, bIsNPC, bIsDead);
	
}

void ACustomCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}



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

void ACustomCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACustomCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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


