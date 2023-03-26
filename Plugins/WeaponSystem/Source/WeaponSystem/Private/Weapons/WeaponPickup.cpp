// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponPickup.h"

#include "Interfaces/CharacterWeaponPickups.h"
#include "Net/UnrealNetwork.h"


AWeaponPickup::AWeaponPickup()
{
	bReplicates = true;
	SetReplicatingMovement(true);
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Gun Mesh");
	GunMesh->SetGenerateOverlapEvents(true);
	GunMesh->SetCollisionProfileName(TEXT("DefaultPickup"));
	GunMesh->SetSimulatePhysics(true);
	GunMesh->SetRenderCustomDepth(true);
	RootComponent = GunMesh;
	GunMesh->bRenderCustomDepth = true;
}

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	if(WeaponPickupData.bHasAutoMode){WeaponPickupData.bIsInAutoMode = true;}
	const UEnum* WeaponTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponName_T"));
	FString EnumConvertedName = WeaponTypeEnum ? WeaponTypeEnum->GetNameStringByIndex(static_cast<uint32>(WeaponPickupData.WeaponType)) : TEXT("<Invalid Enum Type>");
	DisplayName = EnumConvertedName;
	SetMesh();
}

void AWeaponPickup::SetMesh()
{
	if(WeaponPickupData.MeshForPickup != nullptr)
	{
		GunMesh->SetSkeletalMesh(WeaponPickupData.MeshForPickup);
	}
}

void AWeaponPickup::WeaponDroppedEvent(FWeaponData_T DroppedWeaponData)
{
	if(DroppedWeaponData.MeshForPickup == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Data is set in Weapon Pickup"));
	}
	else
	{
		WeaponPickupData = DroppedWeaponData;
		const UEnum* WeaponTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponName_T"));
		FString EnumConvertedName = WeaponTypeEnum ? WeaponTypeEnum->GetNameStringByIndex(static_cast<uint32>(DroppedWeaponData.WeaponType)) : TEXT("<Invalid Enum Type>");
		DisplayName = EnumConvertedName;
		SetMesh();
	}
}

void AWeaponPickup::OnInteract(AActor* Caller)
{
	ICharacterWeaponPickups* CharacterInteracting = Cast<ICharacterWeaponPickups>(Caller);
	if(CharacterInteracting != nullptr)
	{
		if(WeaponPickupData.MeshForPickup != nullptr)
		{
			SetOwner(Caller);
			const int32 RemainingAmmo = CharacterInteracting->PickupGunEvent(WeaponPickupData);
			if(RemainingAmmo)
			{
				WeaponPickupData.TotalAmmoCount = RemainingAmmo;
			}
			else
			{
				CharacterInteracting->DestroyActor(this);
			}
		}
	}
	ReceiveOnInteract(Caller);
}

void AWeaponPickup::OnPickUp(AActor* Caller)
{
	ReceiveOnPickUp(Caller);
}

void AWeaponPickup::StartFocus()
{
	ReceiveStartFocus();
}

void AWeaponPickup::EndFocus()
{
	ReceiveEndFocus();
}

void AWeaponPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponPickup, WeaponPickupData);

}
