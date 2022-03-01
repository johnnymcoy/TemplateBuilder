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
	bPickedup = false;
	RootComponent = GunMesh;
}

void AWeaponPickupBase::BeginPlay()
{
	Super::BeginPlay();
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
			bPickedup = true;
			OnRep_PickedUp();
			ClearMesh();
			GunMesh->SetSkeletalMesh(nullptr);
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

void AWeaponPickupBase::OnRep_PickedUp() 
{
	GunMesh->SetSkeletalMesh(nullptr);
	ClearMesh();
}

void AWeaponPickupBase::ClearMesh_Implementation() 
{
	GunMesh->SetSkeletalMesh(nullptr);
	// UE_LOG(LogTemp,Warning,TEXT("ClearMesh"));
}

void AWeaponPickupBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 	DOREPLIFETIME(AWeaponPickupBase, bPickedup);
}
