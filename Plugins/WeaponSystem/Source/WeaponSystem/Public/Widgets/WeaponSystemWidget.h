// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/WeaponStructs.h"
#include "WeaponSystemWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAutoModeChangedSignature, bool, bIsInAutoMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHolsteredChangedSignature, bool, bIsHolstered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHasGunChangedSignature, bool, bHasGun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsAimingChangedSignature, bool, bIsAiming);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsReloadingChangedSignature, bool, bIsReloading);



UCLASS()
class WEAPONSYSTEM_API UWeaponSystemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerWeaponState(FPlayerWeaponState WeaponState);
	UFUNCTION(BlueprintCallable)
	void UpdateAmmo(int32 CurrentAmmo, int32 TotalAmmo);
	UFUNCTION(BlueprintCallable)
	void UpdateWeapons(TArray<FWeaponData_T> Weapons, int32 CurrentWeaponIndex);
	
	
	UFUNCTION(BlueprintCallable)
	void UpdateWeapon(FWeaponData_T Weapon);

protected:
	virtual bool Initialize() override;

	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FOnAutoModeChangedSignature OnAutoModeChanged;
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FOnHolsteredChangedSignature OnHolsteredChanged;
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FOnHasGunChangedSignature OnHasGunChanged;
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FOnIsAimingChangedSignature OnIsAimingChanged;
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FOnIsReloadingChangedSignature OnIsReloadingChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<struct FWeaponData_T> WeaponsInventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentWeaponEquipped;

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
