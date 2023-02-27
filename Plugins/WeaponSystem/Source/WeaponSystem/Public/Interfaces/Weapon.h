// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/WeaponStructs.h"
#include "Weapon.generated.h"

UINTERFACE(MinimalAPI,meta = (CannotImplementInterfaceInBlueprint))
class UWeapon : public UInterface
{
	GENERATED_BODY()
};

class WEAPONSYSTEM_API IWeapon
{
	GENERATED_BODY()


public:
	// Shooting
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void Fire(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy) = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void Reload(float ReloadTime) = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void CancelReload() = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void SwitchAutoMode() = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void BlindFireWeapon(const float in_Accuracy, const AActor* ActorToIgnore = nullptr) = 0;

	//Extra Functions
	// UFUNCTION(Category = "Weapons")
	// virtual void ToggleFlashlight() = 0;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void MoveUMG(bool bIsRightShoulder) = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void FadeInUMG(bool bIsAiming) = 0;
	
	//Get Weapon infos
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual FWeaponData_T GetWeaponData() = 0;
	// todo maybe add in?
	// UFUNCTION(BlueprintCallable, Category = "Weapons")
	// virtual FWeaponStats_T GetWeaponStats() = 0;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual bool IsInAutoMode() = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual bool IsClipEmpty() = 0;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void SetWeaponData(const FWeaponData_T in_WeaponData) = 0;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Weapons")
	virtual void SetWeaponMesh(USkeletalMesh* SkeletalMesh) = 0;


	///////////////

};
