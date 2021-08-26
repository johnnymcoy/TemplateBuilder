// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ALSV4_CPP/Public/Library/ALSCharacterEnumLibrary.h"
#include "TemplateBuilder/Weapons/WeaponClassIdentifier.h"
#include "CustomStructLibrary.generated.h"


USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()    

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Basics")
	EWeaponName WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Basics")
	EALSOverlayState OverlayState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Basics")
	bool bIsPrimary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 TotalAmmoCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 ClipSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offsets")
	FVector WeaponOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offsets")
	FRotator RotationOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offsets")
	bool bIsLeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extra Stats")
	float Recoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoMode")
	float FireRate;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	class UTexture2D* GunThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	class USkeletalMesh* MeshForPickup;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	TSubclassOf<class AWeaponClassIdentifier> WeaponClass;

};

UENUM(BlueprintType)
enum class EALSInjuredState : uint8
{
	None,
	Head,
	LeftArm,
	RightArm,
	LeftLeg,
	RightLeg,
};

