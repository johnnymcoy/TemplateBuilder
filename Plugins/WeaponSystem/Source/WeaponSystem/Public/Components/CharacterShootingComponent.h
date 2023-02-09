// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterComponent.h"
#include "Data/WeaponStructs.h"
#include "CharacterShootingComponent.generated.h"

USTRUCT(BlueprintType)
struct FPlayerWeaponState
{
	GENERATED_USTRUCT_BODY()    
	UPROPERTY(BlueprintReadWrite)
	bool bPrimaryEquipped;
	UPROPERTY(BlueprintReadWrite)
	bool bSecondaryEquipped;
	UPROPERTY(BlueprintReadWrite)
	bool bIsReloading;
	UPROPERTY(BlueprintReadWrite)
	bool bIsHolstered;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAiming;
	UPROPERTY(BlueprintReadWrite)
	bool bHasGun;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UCharacterShootingComponent : public UCharacterComponent
{
	GENERATED_BODY()


public:
	UCharacterShootingComponent();

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);


	// Main Functions
	void ShootGun();
	void PullTrigger();
	void StopShootGun();
	
	// void AimPressed(bool bRightShoulder);
	// void AimReleased(bool bRightShoulder);

	// HUD / UMG
	void AimPressedAction();
	void AimReleasedAction();
	// // void MoveUMG(bool bRightShoulder);
	// void SetupHUD();

	// Reload
	void Reload();
	void SwitchAutoMode();


	// Swaps / Pickups
	void PickupWeapon(FWeaponData_T WeaponToPickup);
	void AddWeaponToInventory(FWeaponData_T NewWeapon);
	void RemoveWeaponFromInventory(int32 WeaponToRemove);
	void EquipWeapon(FWeaponData_T WeaponToEquip);
	void SwapWeaponPressed();
	void SwapWeaponReleased();
	void SwapWeapon();
	void HolsterWeapon();
	void ThrowWeapon(FWeaponData_T WeaponToThrow);
	void DropWeapon();
	
	// Helper Functions
	void ClearWeapon();

	void Death();

	//Getters
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	FWeaponData_T GetCurrentWeaponData() const{return GetCurrentWeapon();};
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	FPlayerWeaponState GetPlayerWeaponState() const{return PlayerWeaponState;};


protected:
	virtual void BeginPlay() override;

	
	// less than 1 is Low, 5 Is Very high
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", meta=(UIMin = "0.1", UIMax = "10.0"))
	float Accuracy = 1;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Animations")
	UAnimMontage* GetReloadAnimation(EWeaponName_T WeaponName);

private:

	// Weapon Functions
	void ReloadDelay();
	void CancelReload();

	void AddAmmo(const int32 AmountToAdd, const int32 WeaponIndex);


	
	
	//Weapon Activated Functions
	void Recoil();
	//TODO need to get from Character
	// void CalculateAccuracy();
	
	float CalculateAccuracy();

	// Swaps / Pickups
	TArray<FWeaponData_T> WeaponInventory;
	int32 CurrentWeaponIndex = 0;

	FWeaponData_T GetCurrentWeapon() const;

	UPROPERTY()
	AActor* OwnerActor;
	//todo Empty AActor
	AActor* CurrentWeapon;


	// AutoMatic Weapon fire Handler
	FTimerHandle ShootingTimerHandle;
	FTimerHandle ReloadTimerHandle;

	
	FPlayerWeaponState PlayerWeaponState;

	
};
