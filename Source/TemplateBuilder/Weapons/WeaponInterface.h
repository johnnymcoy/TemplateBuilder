// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void GetGunImpulse(float out_GunImpulse, float out_HeadMultiplier);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void SetWeaponAmmo(const int32 in_CurrentAmmo, const int32 in_TotalAmmoCount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void Fire();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	float Reload();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void CancelReload();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void SwitchAutoMode();

	//Get Weapon infos
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	int GetCurrentAmmo();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	int GetTotalAmmoCount();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	bool IsInAutoMode();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
	void GetWeaponInfo();


	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	//void InitaizeWeaponStats();

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	//void HitInfo(const AActor* ActorHit, );



};
