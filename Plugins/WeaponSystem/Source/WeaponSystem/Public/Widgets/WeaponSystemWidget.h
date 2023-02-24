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
	// Todo
	UFUNCTION(BlueprintCallable)
	void SetHolstered(bool In_bIsHolstered);
	// Todo
	UFUNCTION(BlueprintCallable)
	void SetAutoMode(bool In_bIsInAutoMode);
	
	UFUNCTION(BlueprintCallable)
	void UpdateWeapon(FWeaponData_T Weapon);

	UFUNCTION(BlueprintCallable)
	void UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo);

	// Todo
	UFUNCTION(BlueprintCallable)
	void UpdateWeapons(TArray<FWeaponData_T> Weapons);
protected:
	virtual bool Initialize() override;


private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentAmmoTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentTotalAmmoTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentWeaponNameTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* CurrentWeaponThumbnailImage;


	
};
