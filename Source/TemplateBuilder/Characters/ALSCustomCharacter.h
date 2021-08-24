// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ALSCharacter.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "ALSCharacterInterface.h"
#include "ALSCustomCharacter.generated.h"


UCLASS()
class TEMPLATEBUILDER_API AALSCustomCharacter : public AALSCharacter, public IALSCharacterInterface
{
	GENERATED_BODY()

protected: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
	class UChildActorComponent* Gun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PhysicalAnimation")
	class UPhysicalAnimationComponent* PhysicalAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	class USceneComponent* ThrowPoint;

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerDropGun(FWeaponData SpawnWeaponData, FVector Location, FRotator Rotation, FVector ThrowForce);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDropGun(FWeaponData SpawnWeaponData, FVector Location, FRotator Rotation, FVector ThrowForce);

	void WeaponDropped();
	
	//ALS Overriden Functions
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void AimPressedAction();
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void AimReleasedAction();

	void CameraButtonPressed();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	bool bIsAiming;

public:
	// Sets default values for this character's properties
	AALSCustomCharacter(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<class AWeaponPickupBase> GunToSpawn;

	//ALS Overriden Functions
	virtual void RagdollStart() override;
	virtual void RagdollEnd() override;
	void RagdollMeshFix();
	FTimerHandle RagdollGroundTimer;


	/////////////////////////////INTERFACE EVENTS///////////////////////////////////////////////
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Reaction")
	void AddImpulseEvent(const FVector in_Impulse, const FName in_HitBone, const float in_GunImpulse);
	virtual void AddImpulseEvent_Implementation(const FVector in_Impulse, const FName in_HitBone, const float in_GunImpulse);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void BulletDamageEvent(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone, AController* in_EventInstigator, AActor* in_DamageCauser,
    TSubclassOf<class UDamageType> in_DamageType);
	virtual void BulletDamageEvent_Implementation(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone, AController* in_EventInstigator, AActor* in_DamageCauser,
    TSubclassOf<class UDamageType> in_DamageType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickups")
	void PickupGunEvent(const FWeaponData in_WeaponData);
	virtual void PickupGunEvent_Implementation(const FWeaponData in_WeaponData);

	///////////////////////////////////////////////////////////////////////////


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reaction | Injured")
	EALSInjuredState InjuredState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction | Physical Animation")
	FPhysicalAnimationData PelvisData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction | Physical Animation")
	FPhysicalAnimationData SpineData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction | Physical Animation")
	FPhysicalAnimationData LegData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaction | Physical Animation")
	FPhysicalAnimationData ArmData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	float PickupThrowIntensity;

	//Update HUD elements
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void UpdateWBP(FWeaponData WeaponData);
	virtual void UpdateWBP_Implementation(FWeaponData WeaponData);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void PickupGunWBP(FWeaponData WeaponData);
	virtual void PickupGunWBP_Implementation(FWeaponData WeaponData);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void UpdateHealthWBP(float Health, float DefaultHealth, float Shield, float DefaultShield);
	virtual void UpdateHealthWBP_Implementation(float Health, float DefaultHealth, float Shield, float DefaultShield);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void AddRecoilWBP(float RecoilAmmount);
	virtual void AddRecoilWBP_Implementation(float RecoilAmmount);


	//Optimization
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Optimization")
	bool bHasGun;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Optimization")
	bool bIsNPC;
	bool DoOnce = false;
	UPROPERTY(Replicated)
	bool DeathOnce = false;
	
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void PickupWeapon(FWeaponData WeaponData);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickupWeapon(FWeaponData WeaponData);
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwapWeapon();

	bool PrimaryEquiped;
	bool bIsHolstered;

	UFUNCTION(BlueprintCallable, Category = "WeaponSwap")
	void HolsterWeapon();
	UFUNCTION(BlueprintCallable, Category = "WeaponSwap")
	void NoSecondaryEquip();
	UFUNCTION(Server, Reliable)
	void ServerClearWeapon();


	void EquipWeapon(FWeaponData WeaponData);
	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(FWeaponData WeaponData);


	void AddAmmo(FWeaponData WeaponData);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Gun System")
	UAnimMontage* GetReloadAnimation(EWeaponName WeaponName);

	UFUNCTION(BlueprintPure)
	bool IsDead() const;


	UFUNCTION(BlueprintCallable, Category = "Stats")
    void GetThrowStats(FVector &OutLocation, FRotator &OutRotation, FVector &OutScale, FVector &OutThrowForce) const;	

	UPROPERTY(ReplicatedUsing=OnRep_ClearWeapon, EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	struct FWeaponData CurrentWeaponData;

	UFUNCTION()
	void OnRep_ClearWeapon();

	UPROPERTY(BlueprintReadOnly, Category = "Weapons")
	struct FWeaponData PrimaryWeaponData;
	UPROPERTY(BlueprintReadOnly, Category = "Weapons")
	struct FWeaponData SecondaryWeaponData;
	//less than 1 is Low, 5 Is Very high
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Weapon", meta=(UIMin = "0.1", UIMax = "10.0"))
	float Accuracy = 1;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void ThrowWeaponEvent(FWeaponData WeaponData);
	virtual void ThrowWeaponEvent_Implementation(FWeaponData WeaponData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void Death();
	virtual void Death_Implementation();


	// void ServerDeath();
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ShootGun();
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ShootGunCheck();
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void StopShootGun();
protected:
	UFUNCTION()
 	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnHealthChanged(UHealthComponent* HealthComp, float Health, float DefaultHealth, float Shield, float DefaultShield, const class UDamageType* DamageType);


private:


	FTimerHandle ShootingTimerHandle;

	void Recoil();
	float RecoilAmount;
	float DefaultAccuracy;
	void CalculateAccuracy();

	void SwitchAutoMode();

	void SetupPhysicalAnimation();
	void SetupPhysicalAnimationDefaults();

	// UPROPERTY(EditAnywhere)
	// TSubclassOf<class UUserWidget> PauseMenuClass;
	// UPROPERTY()
	// UUserWidget* PauseMenu;

	//Could be Component?
	//Gun Functions 
	// void DropWeapon();

	void Reload();
	void ReloadDelay();
	void CancelReload();

	UFUNCTION(Server, Reliable)
	void ServerPlayMontageAnimation(UAnimMontage* MontageToPlay, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMontageAnimation(UAnimMontage* MontageToPlay, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages);
	UFUNCTION(Server, Reliable)
	void ServerStopMontageAnimation(float InBlendOutTime, const UAnimMontage* Montage);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStopMontageAnimation(float InBlendOutTime, const UAnimMontage* Montage);

	//void MulticastPlayMontageAnimation();

	//Switch Weapon 
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	float WeaponSwitchHoldTime = 0.2f;

	/** Last time the camera action button is pressed */
	float SwapWeaponActionPressedTime = 0.0f;

	void SwapWeaponPressed();
	void SwapWeaponReleased();
	
	/* Timer to manage camera mode swap action */
	FTimerHandle OnWeaponSwapTimer;

	//Vars
	bool bIsReloading;

	FTimerHandle ReloadTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	struct FWeaponData EmptyWeaponData;


	UFUNCTION()
	void AnyDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	void HealthDelay();
	FTimerHandle HealthTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	//Set to Weapon Base
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWeaponBase> WeaponClassRef;

};