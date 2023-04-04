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

	//-		Setup Functions		//
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	void SetThrowPoint(USceneComponent* ThrowPointComponent){ThrowPoint = ThrowPointComponent;};

	//-		Delegates			//
	UPROPERTY(BlueprintAssignable, Category = "Weapons")
	FOnWeaponEqiupped OnWeaponEquipped;
	UPROPERTY(BlueprintAssignable, Category = "Ammo")
	FOnAmmoChangedSignature OnAmmoChanged;
	UPROPERTY(BlueprintAssignable, Category = "Ammo")
	FOnWeaponStateChanged OnWeaponStateChanged;
	UPROPERTY(BlueprintAssignable, Category = "Recoil")
	FOnBulletShot OnBulletShot;
	
	//-		Main Functions		//
	void ShootGun();
	void PullTrigger();
	void StopShootGun();
	
	// HUD / UMG
	// void AimPressed(bool bRightShoulder);
	// void AimReleased(bool bRightShoulder);
	// void MoveUMG(bool bRightShoulder);

	//-		Input Functions		//
	void AimPressedAction();
	void AimReleasedAction();
	void ThrowWeaponAction();
	

	//-			Reload			//
	void Reload();
	void SwitchAutoMode();


	//-		Swaps / Pickups		//
	void PickupWeapon(FWeaponData_T WeaponToPickup, int32& RemainingAmmo);
	void EquipWeapon(FWeaponData_T WeaponToEquip);
	void SwapWeaponPressed();
	void SwapWeaponReleased();
	void SwapWeapon();
	void HolsterWeapon();

	//-			Throw 			//
	void ThrowWeapon(FWeaponData_T WeaponToThrow);
	void DropWeapon(FWeaponData_T WeaponToDrop);

	void GetThrowStats(FTransform& OutTransform, FVector& OutThrowForce) const;
	
	//-			Server			 //
	UFUNCTION(Server, Reliable)
	void ServerShootGun();
	UFUNCTION(Server, Reliable)
	void ServerStopShootGun();
	UFUNCTION(Server, Reliable)
	void Server_SwapWeapon();

	UFUNCTION(Server, Reliable)
	void ServerPickupWeapon(FWeaponData_T WeaponToPickup, int32 RemainingAmmo);
	// UFUNCTION(Server, Reliable)
	// void ServerEquipWeapon(FWeaponData_T WeaponToEquip);

	UFUNCTION(Server, Reliable)
	void ServerThrowWeapon(FWeaponData_T WeaponToThrow);
	//-		Helper Functions	 // 
	UFUNCTION(Server, Reliable)
	void ServerHideWeaponModel(bool bHidden);

	
	// UFUNCTION(Server, Reliable)
	// void ServerEquipWeapon(FWeaponData_T WeaponToEquip);

	

	void OwnerDeath() override;

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

	
	//- less than 1 is Low, 5 Is Very high //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", meta=(UIMin = "0.1", UIMax = "10.0"))
	float Accuracy = 1;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Animations")
	UAnimMontage* GetReloadAnimation(EWeaponName_T WeaponName);



private:

	//-		Weapon Functions	//
	void ReloadDelay();
	void CancelReload();

	//- Stored to be able to call function on 
	UPROPERTY()
	UAnimMontage* ReloadAnimation;

	//-		Helper Functions	//
	bool PlayerHasWeaponOfType(EWeaponName_T WeaponType);
	int32 AddAmmo(const int32 AmountToAdd, const int32 WeaponIndex);

	
	//- Weapon Activated Functions	//
	void Recoil();
	//TODO need to get from Character
	// void CalculateAccuracy();
	float CalculateAccuracy();

	
	//-		Weapon Inventory		//
	void AddWeaponToInventory(FWeaponData_T NewWeapon);
	void RemoveWeaponFromInventory(int32 WeaponToRemove);
	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerAddWeaponToInventory(FWeaponData_T NewWeapon);
	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerRemoveWeaponFromInventory(int32 WeaponToRemove);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_SpawnWeapon(FWeaponData_T WeaponSpawned);

	//-		HUD delay Update	//
	void UpdateHUD();
	void DelayedUpdateHUD();

	//? Testing out replication for the inventory
	//- Replicated Stats	// 
	UPROPERTY(ReplicatedUsing=OnRep_WeaponInventory)
	TArray<FWeaponData_T> WeaponInventory;
	UFUNCTION()
	void OnRep_WeaponInventory();
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentWeaponIndex)
	int32 CurrentWeaponIndex = 0;
	UFUNCTION()
	void OnRep_CurrentWeaponIndex();
	
	UPROPERTY(ReplicatedUsing=OnRep_PlayerWeaponState)
	FPlayerWeaponState PlayerWeaponState;
	UFUNCTION()
	void OnRep_PlayerWeaponState();

	//-		Weapon Spawning/ Pickup			//
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	TSubclassOf<class AWeaponPickup> WeaponToSpawn;
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float PickupThrowIntensity = 500;
	UPROPERTY()
	USceneComponent* ThrowPoint;

	
	//- Last time the camera action button is pressed //
	float SwapWeaponPressedTime = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Switch Weapons")
	float SwapWeaponHoldTime = 0.2f;
	bool bSwapWeaponPressed = false;

	//- Reload Animation Blend out time //
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	float AnimationBlendOutTime = 0.05f;

	UPROPERTY()
	AActor* OwnerActor;

	//-			Timers			//
	//- Automatic Weapon fire	//
	FTimerHandle ShootingTimerHandle;
	FTimerHandle ReloadTimerHandle;
	FTimerHandle WeaponSwapTimerHandle;
	FTimerHandle UpdatedHUDTImerHandle;
	
};
