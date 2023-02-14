// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/WeaponStructs.h"
#include "WeaponSystemWidget.generated.h"

UCLASS()
class WEAPONSYSTEM_API UWeaponSystemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetHolstered(bool In_bIsHolstered);

	UFUNCTION(BlueprintCallable)
	void SetAutoMode(bool In_bIsInAutoMode);
	
	UFUNCTION(BlueprintCallable)
	void UpdateWeapon(FWeaponData_T Weapon);
	
	UFUNCTION(BlueprintCallable)
	void UpdateWeapons(TArray<FWeaponData_T> Weapons);
protected:
	virtual bool Initialize() override;

};
