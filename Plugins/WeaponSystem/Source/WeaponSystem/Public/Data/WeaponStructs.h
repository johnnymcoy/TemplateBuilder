#pragma once
#include "CoreMinimal.h"
#include "WeaponStructs.generated.h"

UENUM(BlueprintType)
enum class EWeaponName_T : uint8
{
	Default,
	Pistol,
	MachinePistolGen1,
	MachinePistolGen2,
	Revolver,
	Rifle1,
	RifleBase,
	RifleLaser,
	RiflePlasma1,
	RiflePlasma2,
	RiflePlasma3,
	RifleSmall,
	Shotgun,
	ShotgunPlasma,
	SMG,
	Sniper,
	SniperPlasma,
	SyringeGun
	//todo Add more
};


USTRUCT(BlueprintType)
struct FWeaponData_T
{
	GENERATED_USTRUCT_BODY()    

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Basics")
	EWeaponName_T WeaponType;
	//Overlay (Animation Pose i.e. Shotgun, 2 Handed Pistol, Rifle)
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Basics")
	// EALSOverlayState OverlayState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsInAutoMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Basics")
	bool bHasAutoMode;
	//todo Array of what attachments are on the weapon	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachments")
	bool bFlashlightOn;

	// Current Clip Ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentAmmo;
	// Total Ammo
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
	//1 = Very Fast, 250 Slow i.e. Sniper// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoMode", meta = (ClampMin = "1", ClampMax = "250", UIMin = "1", UIMax = "250"))
	float FireRate;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	class UTexture2D* GunThumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	class USkeletalMesh* MeshForPickup;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	// TSubclassOf<class AWeaponClassIdentifier> WeaponClass;
	//
	bool IsValid()
	{
		if(MeshForPickup == nullptr)
			// || WeaponClass == nullptr)
		{
			return false;
		}
		return true;
	}
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	bool bIsValid = IsValid();
	
	// Returns Remaining Ammo
	int32 AddAmmo(const int32 AmountToAdd)
	{
		//If the total ammo is less than the max ammo && the total ammo + the pickup ammo is less than the max ammo, simply add them together
		if(TotalAmmoCount < MaxAmmo && (TotalAmmoCount + AmountToAdd) <= MaxAmmo)
		{
			TotalAmmoCount = TotalAmmoCount + AmountToAdd;
			return 0;
		}
		// If the total ammo is less than max, && the amount to pickup will end up more than the Max ammo
		else if(TotalAmmoCount < MaxAmmo && (TotalAmmoCount + AmountToAdd) > MaxAmmo)
		{
			TotalAmmoCount = MaxAmmo;
			const int32 Remainder = (TotalAmmoCount + AmountToAdd) - MaxAmmo;
			return Remainder;
		}
		else // Should only cover if the ammo is Already Max 
		{
			// todo:
			// Should not Destory Actor *(From where it's called(old code))
			return AmountToAdd;
		}
		return 0;
	}
};

USTRUCT(BlueprintType)
struct FWeaponStats_T
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range")
	float GunRange = 200000.0f;

	
};
