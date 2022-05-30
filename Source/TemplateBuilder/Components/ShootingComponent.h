// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/ActorComponent.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "ShootingComponent.generated.h"

//On HUD update event, Sets off blueprint event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChangeSignature, EALSOverlayState, OverlayState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBulletShotSignature, float, RecoilAmount);

// DECLARE_DYNAMIC_MULTICAST_DELEGATE();
USTRUCT(BlueprintType)
struct FPlayerShootingStatus
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


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATEBUILDER_API UShootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShootingComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "Shooting")
	FOnStateChangeSignature OnStateChange;
	//Recalculating Accuracy when needed 
	UPROPERTY(BlueprintAssignable, Category = "Shooting")
	FOnBulletShotSignature OnBulletShot;
	
	//todo Add in Scene components for Weapon Positions
	
	//Weapon Fire
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ShootGun();
	void ValidateShootGun();
	void StopShootGun();

	void AimPressed(bool bRightShoulder);
	void AimReleased(bool bRightShoulder);
	void MoveUMG(bool bRightShoulder);
	void SetupHUD();
	
	//Weapon Swaps/Pickups
	void PickupWeapon(FWeaponData WeaponToPickup);
	void EquipWeapon(FWeaponData WeaponToEquip, bool bPrimaryWeapon = true);
	void SwapWeapon();
	void SwapWeaponPressed();
	void SwapWeaponReleased();
	void HolsterWeapon();
	void ThrowWeapon(FWeaponData WeaponToThrow);
	void DropWeapon(FWeaponData SpawnWeaponData);
	void ClearWeapon();

	//Server Functions//
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickupWeapon(FWeaponData WeaponToPickup);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEquipWeapon(FWeaponData WeaponToEquip, bool bPrimaryWeapon = true);

	
	//Owner Activated Functions
	void SwitchAutoMode();
	void Reload(UAnimMontage* ReloadAnimation);

	void Death();
	
	//Setters
	void SetGunChildActor(UChildActorComponent* GunChildActor){GunChildActorReference = GunChildActor;};
	void SetThrowPoint(USceneComponent* ThrowPointComponent){ThrowPoint = ThrowPointComponent;};
	void SetOwnerMesh(USkeletalMeshComponent* SkeletalMesh){OwnerMesh = SkeletalMesh;};
	void SetAnimInstance(UAnimInstance* AnimationInstance){MainAnimInstance = AnimationInstance;};
	void SetController(AController* Controller);
	void SetAccuracy(float In_Accuracy){Accuracy = In_Accuracy;};
	void SetIsNPC(bool in_bIsNPC){bIsNPC = in_bIsNPC;};
	
	//Getters
	FWeaponData GetCurrentWeaponData() const{return CurrentWeaponData;};
	UFUNCTION(BlueprintCallable)
	FPlayerShootingStatus GetPlayerShootingStatus() const {return PlayerShootingStatus;};
	
protected:

	virtual void BeginPlay() override;

	//Weapon
	UPROPERTY(Replicated, BlueprintReadOnly)
	struct FWeaponData CurrentWeaponData;
	TArray<FWeaponData> CurrentWeaponListData;
	
private:
	UPROPERTY(Replicated)
	FPlayerShootingStatus PlayerShootingStatus;
	//Already has weapon
	void AddAmmo(const FWeaponData in_WeaponData, const bool in_bIsPrimary = true);

	void ReloadDelay();
	void CancelReload();

	void DelayUpdateWeaponHUD();
	void UpdateWeaponHUD();

	FTimerHandle ShootingTimerHandle;
	FTimerHandle ReloadTimer;
	FTimerHandle HUDUpdateTimerHandle;

	UPROPERTY()
	UAnimMontage* LastReloadAnimation;

	//Weapon Activated Functions
	void Recoil();
	//TODO need to get from Character
	// void CalculateAccuracy();

	void GetThrowStats(FTransform &OutTransform, FVector &OutThrowForce) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Weapon")
	float PickupThrowIntensity = 500;

	// bool bPrimaryEquipped;
	// bool bSecondaryEquipped;

	bool bSwapWeaponPressed;
	bool bIsNPC;

	//Default
	float Accuracy = 1;
	
	//Used to swap to, etc. should never change
	const struct FWeaponData EmptyWeaponData;
	
	//todo: Remove
	struct FWeaponData PrimaryWeaponData;
	struct FWeaponData SecondaryWeaponData;

	void AddWeaponToList(FWeaponData SelectedWeapon, bool bAddWeapon);

	//When changed on Owner Activate Component
	// UPROPERTY(VisibleAnywhere)
	// bool bHasGun;
	
	//Owner Stats
	float RecoilAmount;
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	float WeaponSwitchHoldTime = 0.2f;
	
	/** Last time the camera action button is pressed */
	float SwapWeaponActionPressedTime = 0.0f;
	
	/* Timer to manage weapon mode swap action */
	FTimerHandle OnWeaponSwapTimer;
	UPROPERTY(Replicated)
	UChildActorComponent* GunChildActorReference;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<class AWeaponPickupBase> GunToSpawn;
	
	//Weapon HUD
	UPROPERTY()
	class UWeaponWidget* WeaponWidget;
	// UPROPERTY()
	// class UUserWidget* WeaponWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WeaponWidgetClass;
	
	//Owner References
	UPROPERTY()
	USceneComponent* ThrowPoint;
	UPROPERTY()
	USkeletalMeshComponent* OwnerMesh;
	UPROPERTY()
	UAnimInstance* MainAnimInstance;
	
	UPROPERTY()
	AController* OwnerController;
	UPROPERTY()
	APlayerController* OwnerPlayerController;
	UPROPERTY()
	AAIController* OwnerAIController;

	//Animations
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

};

