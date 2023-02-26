// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterComponent.h"
#include "Data/WeaponStructs.h"
#include "CharacterShootingComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponEqiupped, TArray<FWeaponData_T>, Weapons, int32, CurrentWeaponIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedSignature, int32, CurrentAmmo, int32, TotalAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponStateChanged, FPlayerWeaponState, WeaponState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBulletShot, float, RecoilAmount);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WEAPONSYSTEM_API UCharacterShootingComponent : public UCharacterComponent
{
	GENERATED_BODY()


public:
	UCharacterShootingComponent();

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	void SetThrowPoint(USceneComponent* ThrowPointComponent){ThrowPoint = ThrowPointComponent;};

	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FOnWeaponEqiupped OnWeaponEquipped;
	UPROPERTY(BlueprintAssignable, Category = "Ammo")
	FOnAmmoChangedSignature OnAmmoChanged;
	UPROPERTY(BlueprintAssignable, Category = "Ammo")
	FOnWeaponStateChanged OnWeaponStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Recoil")
	FOnBulletShot OnBulletShot;

	// Main Functions
	void ShootGun();
	void PullTrigger();
	void StopShootGun();
	
	// void AimPressed(bool bRightShoulder);
	// void AimReleased(bool bRightShoulder);

	// HUD / UMG
	void AimPressedAction();
	void AimReleasedAction();
	void ThrowWeaponAction();
	// // void MoveUMG(bool bRightShoulder);
	

	// Reload
	void Reload();
	void SwitchAutoMode();


	// Swaps / Pickups
	void PickupWeapon(FWeaponData_T WeaponToPickup,bool& bWeaponWeHave);

	void AddWeaponToInventory(FWeaponData_T NewWeapon);
	void RemoveWeaponFromInventory(int32 WeaponToRemove);
	void EquipWeapon(FWeaponData_T WeaponToEquip);
	void SwapWeaponPressed();
	void SwapWeaponReleased();
	void SwapWeapon();
	void HolsterWeapon();
	void ThrowWeapon(FWeaponData_T WeaponToThrow);
	void DropWeapon(FWeaponData_T WeaponToDrop);

	void GetThrowStats(FTransform& OutTransform, FVector& OutThrowForce) const;
	
	//- Server //
	UFUNCTION(Server, Reliable)
	void ServerPickupWeapon(FWeaponData_T WeaponToPickup,bool bWeaponWeHave);
	//- Helper Functions // 
	UFUNCTION(Server, Reliable)
	void ServerHideWeaponModel(bool bHidden);
	// UFUNCTION(Server, Reliable)
	// void ServerEquipWeapon(FWeaponData_T WeaponToEquip);

	

	void Death();

	//Getters
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	bool GetCurrentWeaponData(FWeaponData_T& CurrentWeaponData) const;
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	void UpdateWeaponInventory();

	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	FPlayerWeaponState GetPlayerWeaponState() const{return PlayerWeaponState;};

	//? Temp Vars 
	//todo Empty AActor
	//? Testing Replicated
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	AActor* CurrentWeapon;



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

	//- Stored to be able to call function on 
	UPROPERTY()
	UAnimMontage* ReloadAnimation;

	
	bool PlayerHasWeaponOfType(EWeaponName_T WeaponType);

	void AddAmmo(const int32 AmountToAdd, const int32 WeaponIndex);


	
	
	//Weapon Activated Functions
	void Recoil();
	//TODO need to get from Character
	// void CalculateAccuracy();
	
	float CalculateAccuracy();

	// Swaps / Pickups
	TArray<FWeaponData_T> WeaponInventory;
	int32 CurrentWeaponIndex = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<class AWeaponPickup> WeaponToSpawn;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float PickupThrowIntensity = 500;

	
	
	//- Last time the camera action button is pressed //
	float SwapWeaponPressedTime = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Switch Weapons")
	float SwapWeaponHoldTime = 0.2f;

	//- Reload Animation Blend out time //
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	float AnimationBlendOutTime = 0.05f;


	bool bSwapWeaponPressed = false;

	// FWeaponData_T GetCurrentWeapon() const;

	UPROPERTY()
	AActor* OwnerActor;
	UPROPERTY()
	USceneComponent* ThrowPoint;


	// AutoMatic Weapon fire Handler
	FTimerHandle ShootingTimerHandle;
	FTimerHandle ReloadTimerHandle;
	FTimerHandle WeaponSwapTimerHandle;

	
	FPlayerWeaponState PlayerWeaponState;

	
};
