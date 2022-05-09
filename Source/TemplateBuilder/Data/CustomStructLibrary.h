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
	//Overlay (Animation Pose i.e. Shotgun, 2 Handed Pistol, Rifle)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Basics")
	EALSOverlayState OverlayState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsInAutoMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Basics")
	bool bHasAutoMode;
	
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

	bool IsValid()
	{
		if(MeshForPickup == nullptr || WeaponClass == nullptr)
		{
			return false;
		}
		return true;
	}
};

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_USTRUCT_BODY()    

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BulletSpread = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DefaultDamage = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float HeadMultiplier = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float GunImpulse = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<class UDamageType> DamageType;
	
	//1 = Very Fast, 250 Slow i.e. Sniper// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoMode", meta = (ClampMin = "1", ClampMax = "250", UIMin = "1", UIMax = "250"))
	float FireRate = 1;
	
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

