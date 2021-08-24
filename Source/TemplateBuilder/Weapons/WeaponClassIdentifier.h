// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponClassIdentifier.generated.h"

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
	//Add more

};

UCLASS()
class TEMPLATEBUILDER_API AWeaponClassIdentifier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponClassIdentifier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
