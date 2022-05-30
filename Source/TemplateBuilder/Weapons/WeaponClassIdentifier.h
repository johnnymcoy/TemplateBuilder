// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponClassIdentifier.generated.h"

// This is a base class used to be able to include WeaponNames to a struct without adding all the weaponBase data
UENUM(BlueprintType)
enum class EWeaponName : uint8
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

UCLASS()
class TEMPLATEBUILDER_API AWeaponClassIdentifier : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponClassIdentifier();

};
