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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	class UShootingComponent* ShootingComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	class UShootingReplicationComponent* ShootingReplication;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	virtual void BeginPlay() override;
	
	//ALS Overriden Functions
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void AimPressedAction() override;
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void AimReleasedAction() override;

	void CameraButtonPressed();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	bool bIsAiming;

public:
	AALSCustomCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<class AWeaponPickupBase> GunToSpawn;

	//ALS Overriden Functions
	virtual void RagdollStart() override;
	virtual void RagdollEnd() override;
	void RagdollMeshFix();
	FTimerHandle RagdollGroundTimer;


	/////////////////////////////INTERFACE EVENTS///////////////////////////////////////////////
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Reaction")
	UFUNCTION(BlueprintCallable, Category = "Reaction")
	// void AddImpulseEvent(const FVector in_Impulse, const FName in_HitBone, const float in_GunImpulse);
	virtual void AddImpulseEvent_Implementation(const FVector in_Impulse, const FName in_HitBone, const float in_GunImpulse) override;

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	// void BulletDamageEvent(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone, AController* in_EventInstigator, AActor* in_DamageCauser,
	 //    TSubclassOf<class UDamageType> in_DamageType);
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void BulletDamageEvent(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone, AController* in_EventInstigator, AActor* in_DamageCauser,
    TSubclassOf<class UDamageType> in_DamageType) override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Damage", meta=(DisplayName = "Bullet Damage"))
	void RecieveBulletDamageEvent(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone, AController* in_EventInstigator, AActor* in_DamageCauser,
	TSubclassOf<class UDamageType> in_DamageType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickups")
	void PickupGunEvent(const FWeaponData in_WeaponData);
	virtual void PickupGunEvent_Implementation(const FWeaponData in_WeaponData) override;
	
	UFUNCTION(Server, Reliable)
	virtual void DestroyActor(AActor* ActorToDestroy) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void DestroyActorOnClient(AActor* ActorToDestroy) override; 

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
	void UpdateHealthWBP(float Health, float DefaultHealth, float Shield, float DefaultShield);
	virtual void UpdateHealthWBP_Implementation(float Health, float DefaultHealth, float Shield, float DefaultShield);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void AddRecoilWBP(float RecoilAmount);
	virtual void AddRecoilWBP_Implementation(float RecoilAmount);

	//Optimization
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Optimization")
	bool bHasGun;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Optimization")
	bool bIsNPC;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Optimization")
	APlayerController* OwnerPlayerController;
	bool DoOnce = false;
	UPROPERTY(Replicated)
	bool DeathOnce = false;
	
	bool bIsHolstered;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Gun System")
	UAnimMontage* GetReloadAnimation(EWeaponName WeaponName);

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION()
	bool IsCrouching() const;
	
	// UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	// struct FWeaponData CurrentWeaponData;


	 // less than 1 is Low, 5 Is Very high
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Weapon", meta=(UIMin = "0.1", UIMax = "10.0"))
	 float Accuracy = 1;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void ThrowWeaponEvent(FWeaponData WeaponData);
	virtual void ThrowWeaponEvent_Implementation(FWeaponData WeaponData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void Death();
	virtual void Death_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ShootGun();
protected:
	UFUNCTION()
 	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnHealthChanged(class UHealthComponent* HealthComp, float Health, float DefaultHealth, float Shield, float DefaultShield, const class UDamageType* DamageType);

	UFUNCTION()
	void OnStateChanged(EALSOverlayState NewOverlayState);

private:
	
	/////////Input Functions/////////
	void FireButtonPressed();
	void FireButtonReleased();
	void ReloadButtonPressed();
	
	void SwapWeaponPressed();
	void SwapWeaponReleased();
	
	void SwitchAutoModePressed();
	void ThrowWeaponPressed();
	////////////////////////////////
	
	FTimerHandle ShootingTimerHandle;
	FTimerHandle HudUpdateHandle;

	UPROPERTY(VisibleAnywhere)
	float ShieldTimeToRegen;
	FTimerHandle ShieldTimeToRegenHandle;
	void ShieldRegen();


	float DefaultAccuracy;
	void CalculateAccuracy();
	UFUNCTION()
	void AddRecoil(float RecoilAmount);
	
	void SetupPhysicalAnimation();
	void SetupPhysicalAnimationDefaults();

	// UPROPERTY(EditAnywhere)
	// TSubclassOf<class UUserWidget> PauseMenuClass;
	// UPROPERTY()
	// UUserWidget* PauseMenu;

	//Server//
	UFUNCTION(Server, Reliable)
	void ServerPlayMontageAnimation(UAnimMontage* MontageToPlay, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMontageAnimation(UAnimMontage* MontageToPlay, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages);
	UFUNCTION(Server, Reliable)
	void ServerStopMontageAnimation(float InBlendOutTime, const UAnimMontage* Montage);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStopMontageAnimation(float InBlendOutTime, const UAnimMontage* Montage);

	
	//Switch Weapons
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	float WeaponSwitchHoldTime = 0.2f;
	/** Last time the weapon action button is pressed */
	float SwapWeaponActionPressedTime = 0.0f;
	/* Timer to manage weapon mode swap action */
	FTimerHandle OnWeaponSwapTimer;

	//Reloading
	bool bIsReloading;
	FTimerHandle ReloadTimer;

	//
	virtual void NotifyHit(class UPrimitiveComponent * MyComp, AActor * Other,class UPrimitiveComponent * OtherComp,	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,const FHitResult & Hit) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	struct FWeaponData EmptyWeaponData;

	//Health
	UFUNCTION()
	void AnyDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	void HealthDelay();
	FTimerHandle HealthTimer;
	
	//Set to Weapon Base
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWeaponBase> WeaponClassRef;
};
