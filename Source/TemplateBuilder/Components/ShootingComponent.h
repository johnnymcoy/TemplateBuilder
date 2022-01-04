// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "ShootingComponent.generated.h"

//On HUD update event, Sets off blueprint event
// DECLARE_DYNAMIC_MULTICAST_DELEGATE();

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATEBUILDER_API UShootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShootingComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//todo Add in Scene components for Weapon Positions
	
	//Weapon Fire
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ShootGun();
	void ValidateShootGun();
	void StopShootGun();

	void AimPressed(bool bRightShoulder);
	void AimReleased(bool bRightShoulder);
	void MoveUMG(bool bRightShoulder);
	

	//Weapon Swaps/Pickups
	void PickupWeapon(FWeaponData WeaponToPickup);
	void EquipWeapon(FWeaponData WeaponToEquip);
	void SwapWeapon();
	void SwapWeaponPressed();
	void SwapWeaponReleased();
	void HolsterWeapon();
	void ThrowWeapon(FWeaponData WeaponToThrow);
	void DropWeapon(FWeaponData SpawnWeaponData);
	void ClearWeapon();

	void UpdateWeaponHUD();
	
	//Owner Activated Functions
	void SwitchAutoMode();
	void Reload(UAnimMontage* ReloadAnimation);
	void ReloadDelay();
	void CancelReload();

	void Death();

	//Setters
	void SetGunChildActor(UChildActorComponent* GunChildActor){GunChildActorReference = GunChildActor;};
	void SetThrowPoint(USceneComponent* ThrowPointComponent){ThrowPoint = ThrowPointComponent;};
	void SetOwnerMesh(USkeletalMeshComponent* SkeletalMesh){OwnerMesh = SkeletalMesh;};
	void SetAnimInstance(UAnimInstance* AnimationInstance){MainAnimInstance = AnimationInstance;};

	//Getters
	FWeaponData GetCurrentWeaponData() const{return CurrentWeaponData;};

	
	bool bIsReloading;

	//Stats
	//less than 1 is Low, 5 Is Very high
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Weapon", meta=(UIMin = "0.1", UIMax = "10.0"))
	float Accuracy;
	
	bool bIsHolstered;
protected:
	bool bWeaponEquiped;
	//Get rid of Primary
	bool PrimaryEquiped;


	virtual void BeginPlay() override;

private:
	//Already has weapon
	void AddAmmo(FWeaponData WeaponData);

	FTimerHandle ShootingTimerHandle;
	FTimerHandle ReloadTimer;

	UAnimMontage* LastReloadAnimation;

	//Weapon Activated Functions
	void Recoil();
	//TODO need to get from Character
	// void CalculateAccuracy();

	void GetThrowStats(FTransform &OutTransform, FVector &OutThrowForce) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Weapon")
	float PickupThrowIntensity = 500;
	

	bool bIsAiming;
	// bool bIsRealoding;
	

	//Weapon
	struct FWeaponData CurrentWeaponData;
	TArray<FWeaponData> CurrentWeaponListData;
	//Used to swap to, etc. should never change
	const struct FWeaponData EmptyWeaponData;
	
	//todo: Remove
	struct FWeaponData PrimaryWeaponData;
	struct FWeaponData SecondaryWeaponData;

	void AddWeaponToList(FWeaponData SelectedWeapon, bool bAddWeapon);

	//When changed on Owner Activate Component
	UPROPERTY(VisibleAnywhere)
	bool bHasGun;
	
	//Owner Stats
	float RecoilAmount;
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	float WeaponSwitchHoldTime = 0.2f;
	
	/** Last time the camera action button is pressed */
	float SwapWeaponActionPressedTime = 0.0f;
	
	/* Timer to manage weapon mode swap action */
	FTimerHandle OnWeaponSwapTimer;
	UPROPERTY()
	UChildActorComponent* GunChildActorReference;
	
	USceneComponent* ThrowPoint;
	UPROPERTY()
	AActor* GunReference;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<class AWeaponPickupBase> GunToSpawn;
	UPROPERTY()
	USkeletalMeshComponent* OwnerMesh;
	UPROPERTY()
	UAnimInstance* MainAnimInstance;

	UPROPERTY(EditAnywhere, Category = "Animation | Reloads")
	TArray<UAnimMontage*> PistolReload;
	UPROPERTY(EditAnywhere, Category = "Animation | Reloads")
	TArray<UAnimMontage*> RifleReload;
	UPROPERTY(EditAnywhere, Category = "Animation | Reloads")
	TArray<UAnimMontage*> ShotgunReload;
	UPROPERTY(EditAnywhere, Category = "Animation | Reloads")
	TArray<UAnimMontage*> RevolverReload;
	UPROPERTY(EditAnywhere, Category = "Animation | Reloads")
	TArray<UAnimMontage*> SniperReload;
	UPROPERTY(EditAnywhere, Category = "Animation | Reloads")
	TArray<UAnimMontage*> SMGReload;

	//Try to get rid of
	
	//Set to Weapon Base
	// UPROPERTY(EditDefaultsOnly)
	// TSubclassOf<class AWeaponBase> WeaponClassRef;


};

