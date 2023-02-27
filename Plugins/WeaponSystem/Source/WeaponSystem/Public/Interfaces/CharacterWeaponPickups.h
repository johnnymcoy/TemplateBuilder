// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponStructs.h"
#include "UObject/Interface.h"
#include "CharacterWeaponPickups.generated.h"

UINTERFACE(MinimalAPI,meta = (CannotImplementInterfaceInBlueprint))
class UCharacterWeaponPickups : public UInterface
{
	GENERATED_BODY()
};

class WEAPONSYSTEM_API ICharacterWeaponPickups
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Pickups")
	virtual int32 PickupGunEvent(const FWeaponData_T In_WeaponData) = 0;

	UFUNCTION(Server, Reliable)
	virtual void DestroyActor(AActor* ActorToDestroy);
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void DestroyActorOnClient(AActor* ActorToDestroy);

};
