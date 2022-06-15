// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,meta = (CannotImplementInterfaceInBlueprint))
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class TEMPLATEBUILDER_API IWeaponInterface
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void GetTraceParams(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy) = 0;

	/// Interface Functions ///
	// Shooting
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void Fire() = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void Reload(float ReloadTime) = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void CancelReload() = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void SwitchAutoMode() = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void BlindFireWeapon() = 0;

	//Extra Functions
	UFUNCTION(Category = "Weapons")
	virtual void ToggleFlashlight() = 0;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void MoveUMG(bool bIsRightShoulder) = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void FadeInUMG(bool bIsAiming) = 0;
	
	//Get Weapon infos
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual FWeaponData GetWeaponData() = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual bool IsInAutoMode() = 0;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void SetWeaponData(const FWeaponData in_WeaponData) = 0;

	///////////////
	
};
