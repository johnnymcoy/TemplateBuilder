// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterShootingComponent.h"
#include "PlayerCharacterShootingComponent.generated.h"

UCLASS()
class WEAPONSYSTEM_API UPlayerCharacterShootingComponent : public UCharacterShootingComponent
{
	GENERATED_BODY()
public:

	UPlayerCharacterShootingComponent();
	
	void SetupWeaponWidget();
	
	void SetupWeaponCrosshairWidget();

protected:
	virtual void BeginPlay() override;

	//- Used for testing, will only update equipped weapon //
	UFUNCTION(BlueprintCallable)
	void UpdateCurrentWeapon();

private:
	// - Weapon System Widget Functions Bind //
	UFUNCTION()
	void AmmoChanged(int32 CurrentAmmo, int32 TotalAmmo);
	UFUNCTION()
	void WeaponEquipped(TArray<FWeaponData_T> Weapons, int32 in_CurrentWeaponIndex);
	UFUNCTION()
	void WeaponStateChanged(FPlayerWeaponState WeaponState);
	//- Crosshair Widget Function Bind // 
	UFUNCTION()
	void BulletShot(float RecoilAmount);
	

	//- Widgets // 
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UWeaponSystemWidget> WeaponWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCrosshairWidget> CrosshairWidgetClass;

	UPROPERTY()
	class UWeaponSystemWidget* WeaponWidget;
	UPROPERTY()
	class UCrosshairWidget* CrosshairWidget;


};

