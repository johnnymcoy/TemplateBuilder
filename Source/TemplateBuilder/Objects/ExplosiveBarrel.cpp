// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "TemplateBuilder/Health/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Net/UnrealNetwork.h"

AExplosiveBarrel::AExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = false;
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	RootComponent = BodyMesh;
	BodyMesh->SetSimulatePhysics(true);
	BodyMesh->SetCollisionObjectType(ECC_PhysicsBody);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &AExplosiveBarrel::OnHealthChanged);
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force Component"));
	RadialForceComponent->SetupAttachment(BodyMesh);
	RadialForceComponent->Radius = 250;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->bIgnoreOwningActor = true;
	bReplicates = true;
	ExplosionImpulse = 400;
	ExplosionScale = 5;
}

void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
}

void AExplosiveBarrel::OnHealthChanged(UHealthComponent* HealthComp, float Health, float DefaultHealth, float Shield,
	float DefaultShield, const UDamageType* DamageType)
{
	if(Health <= 0.0f && !bDied)
	{
		bDied = true;
		OnRep_Exploded();
		FVector ExplosionForce = FVector::UpVector * ExplosionImpulse;
		BodyMesh->AddImpulse(ExplosionForce, NAME_None, true);
		RadialForceComponent->FireImpulse();
	}
}

void AExplosiveBarrel::OnRep_Exploded() 
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorLocation(), FRotator::ZeroRotator, FVector((ExplosionScale)));
	BodyMesh->SetMaterial(0, BarrelExplodedMaterial);
}

void AExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AExplosiveBarrel, bDied);
}