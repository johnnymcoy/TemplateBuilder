// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickupBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "TemplateBuilder/Characters/ALSCharacterInterface.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeaponPickupBase::AWeaponPickupBase()
{
	bReplicates = true;
	SetReplicatingMovement(true);
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>("GunMesh");
    GunMesh->SetGenerateOverlapEvents(true);
	GunMesh->SetCollisionProfileName(TEXT("DefaultPickup"));
	GunMesh->SetSimulatePhysics(true);
	GunMesh->SetRenderCustomDepth(true);
	//GunMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = GunMesh;
	bReplicates = true;
}

void AWeaponPickupBase::BeginPlay()
{
	Super::BeginPlay();
	if(WeaponPickupData.bHasAutoMode){WeaponPickupData.bIsInAutoMode = true;}
	SetMesh();
}

void AWeaponPickupBase::WeaponDroppedEvent(FWeaponData DroppedWeaponData) 
{
	if(DroppedWeaponData.MeshForPickup == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Data in WeaponData"));
	}
	else
	{
		WeaponPickupData = DroppedWeaponData;
		SetMesh();
	}
}

void AWeaponPickupBase::OnInteract_Implementation(AActor* Caller) 
{
	IALSCharacterInterface* CharacterRef = Cast<IALSCharacterInterface>(Caller);
	if(CharacterRef)
	{
		if(WeaponPickupData.MeshForPickup)
		{
			SetOwner(Caller);
			CharacterRef->Execute_PickupGunEvent(Caller, WeaponPickupData);
			CharacterRef->DestroyActor(this);
		}
	}
}

void AWeaponPickupBase::OnPickUp_Implementation(AActor* Caller) 
{

}

void AWeaponPickupBase::StartFocus_Implementation() 
{
	
}

void AWeaponPickupBase::EndFocus_Implementation() 
{
	
}

void AWeaponPickupBase::SetMesh() 
{
	if(WeaponPickupData.MeshForPickup != nullptr)
	{
		GunMesh->SetSkeletalMesh(WeaponPickupData.MeshForPickup);
	}
}
