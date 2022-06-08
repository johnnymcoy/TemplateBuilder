// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "Kismet/GameplayStatics.h"
#include "TemplateBuilder/Characters/ALSCharacterInterface.h"


AHealthPickup::AHealthPickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);
	bReplicates = true;
}

void AHealthPickup::OnInteract(AActor* Caller)
{
	Super::OnInteract(Caller);
	IALSCharacterInterface* CharacterRef = Cast<IALSCharacterInterface>(Caller);
	if(CharacterRef)
	{
		UGameplayStatics::ApplyDamage(Caller, -HealthToGain, Caller->GetInstigatorController(), this, DamageType);
		CharacterRef->DestroyActor(this);
	}
	PlayParticleEffects();
}

void AHealthPickup::PlayParticleEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupEffects, GetActorLocation());
}
