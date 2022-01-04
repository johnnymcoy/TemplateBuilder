// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "ALSCharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UALSCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class TEMPLATEBUILDER_API IALSCharacterInterface
{
	GENERATED_BODY()

public:
	//Add extra Vars into this, so can be used for dismemberment?
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Reaction")
	void AddImpulseEvent(const FVector in_Impulse, const FName in_HitBone, const float in_GunImpulse);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void BulletDamageEvent(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone, AController* in_EventInstigator, AActor* in_DamageCauser,
    TSubclassOf<class UDamageType> in_DamageType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickups")
	void PickupGunEvent(const FWeaponData in_WeaponData);


};
