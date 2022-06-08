// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickupBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "TemplateBuilder/Characters/ALSCharacterInterface.h"

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

void AWeaponPickupBase::OnInteract(AActor* Caller) 
{
	IALSCharacterInterface* CharacterRef = Cast<IALSCharacterInterface>(Caller);
	if(CharacterRef)
	{
		if(WeaponPickupData.MeshForPickup)
		{
			SetOwner(Caller);
			CharacterRef->PickupGunEvent(WeaponPickupData);
			CharacterRef->DestroyActor(this);
		}
	}
	ReceiveOnInteract(Caller);
}

void AWeaponPickupBase::OnPickUp(AActor* Caller) 
{
	ReceiveOnPickUp(Caller);

}

void AWeaponPickupBase::StartFocus() 
{
	ReceiveStartFocus();

}

void AWeaponPickupBase::EndFocus() 
{
	ReceiveEndFocus();
}

void AWeaponPickupBase::SetMesh() 
{
	if(WeaponPickupData.MeshForPickup != nullptr)
	{
		GunMesh->SetSkeletalMesh(WeaponPickupData.MeshForPickup);
	}
}
