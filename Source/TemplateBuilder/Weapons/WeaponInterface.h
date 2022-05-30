// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class TEMPLATEBUILDER_API IWeaponInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void GetTraceParams(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy);

	/// Interface Functions ///
	// Shooting
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void Fire();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void Reload(float ReloadTime);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void CancelReload();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void SwitchAutoMode();
	UFUNCTION(Category = "Weapons")
	virtual void BlindFireWeapon() = 0;

	//Extra Functions
	UFUNCTION(Category = "Weapons")
	virtual void ToggleFlashlight() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void MoveUMG(bool bIsRightShoulder);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void FadeInUMG(bool bIsAiming);
	
	//Get Weapon infos
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	FWeaponData GetWeaponData();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	bool IsInAutoMode();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void SetWeaponData(const FWeaponData in_WeaponData);

	///////////////
	
};
