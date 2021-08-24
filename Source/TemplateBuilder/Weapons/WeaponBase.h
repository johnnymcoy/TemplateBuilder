// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TemplateBuilder/Weapons/WeaponClassIdentifier.h"
#include "ALSV4_CPP/Public/Library/ALSCharacterEnumLibrary.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "WeaponInterface.h"
#include "TemplateBuilder/Debug/DebugInterface.h"
#include "WeaponBase.generated.h"

//Weapon Interface 

// Contatins infromation of a single Hitscan Weapon Linetrace
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int a;
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;
	UPROPERTY()
	FVector_NetQuantize TraceTo;
};


UCLASS()
class TEMPLATEBUILDER_API AWeaponBase : public AWeaponClassIdentifier, public IWeaponInterface, public IDebugInterface
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	//Components//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USkeletalMeshComponent* GunMeshComponent; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USceneComponent* Muzzle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayFireEffects(FVector TraceEnd);
	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	//Server Playing the 
	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;
	UFUNCTION()
	void OnRep_HitScanTrace();

	//Ammo Counter 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USceneComponent* UMG_RightLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USceneComponent* UMG_LeftLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UWidgetComponent* AmmoWidgetComponent;

public:	
	//Base Functions//
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon Functions")
	void Fire();
	virtual void Fire_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon Functions")
	float Reload();
	float Reload_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void CancelReload();
	void CancelReload_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon Functions")
	void SwitchAutoMode();
	virtual void SwitchAutoMode_Implementation();
	//Multiplayer
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShoot();
	UFUNCTION(Server, Reliable)
	void ServerReload();



	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBulletDamage(FHitResult Hit, FVector in_Impulse);


	//Could Remove these and replace with a function that returns WeaponData?
	//Extra Functions
	int GetCurrentAmmo_Implementation();
	int GetTotalAmmoCount_Implementation();

	//Pickups
	virtual void GetWeaponInfo_Implementation();

	bool IsInAutoMode_Implementation();



	//Ammo UMG
	UFUNCTION()
	void FadeInUMG(float Alpha);
	UFUNCTION()
	void MoveUMG(bool bRightShoulder);

	//Blueprint Pure Function//
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon Functions")
	void UpdateAmmoUMG();
	virtual void UpdateAmmoUMG_Implementation();


	///Interface Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void GetTraceParams(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy);
	virtual void GetTraceParams_Implementation(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void GetGunImpulse(float out_GunImpulse, float out_HeadMultiplier);
	virtual void GetGunImpulse_Implementation(float out_GunImpulse, float out_HeadMultiplier);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
	void SetWeaponAmmo(const int32 in_CurrentAmmo, const int32 in_TotalAmmoCount);
	virtual void SetWeaponAmmo_Implementation(const int32 in_CurrentAmmo, const int32 in_TotalAmmoCount);

	UFUNCTION(Server, Reliable)
	void ServerGetTraceParams(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy);

	//Debug Interface
	virtual void EnableWeaponDebug_Implementation(bool DebugStatus);


private:

	void Shoot();
	void ReloadDelay();

	UFUNCTION()
	bool LineTrace(FHitResult& Hit, FVector& ShotDirection);
	
	//bool LineTrace(FHitResult& Hit, FVector& ShotDirection);


	void CalculateBulletSpread(FVector& NewBulletSpread);

	bool CanShoot();

	bool IsReloading;
	FTimerHandle ReloadTimer;
	float GunRange = 200000.0f;
	UPROPERTY(Replicated)
	FVector TraceLocation;
	FRotator TraceRotation;
	float AccuracyMultiplier;
	const class AActor* TraceActorToIgnore; 

	//Muzzle Flash//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// class UStaticMeshComponent* MuzzleFlash;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// class UPointLightComponent* PointLight;


	///////Debug///////
	bool bDebuggingMode;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	FWeaponData GunWeaponData;

	//Base Variables//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BulletSpread;


	//bool IsOwnerAI;
	// for changing the Bullet spread // 

	//Weapon Damage 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DefaultDamage;
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float HeadMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float GunImpulse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<class UDamageType> DamageType;

	//Ammo
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int CurrentAmmoCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int TotalAmmoCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int ClipSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float Recoil;

	// Can change // to int? or from Weapon Name? 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool IsPrimary;

	// for changing the Bullet spread // 
	//and for linetrace//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool PlayerOwns;

	//Weapon Name (Display Name)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EWeaponName WeaponName;

	//Overlay (Animation Pose i.e. Shotgun, 2 Handed Pistol, Rifle)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EALSOverlayState OverlayState;

	//for Automatic Weapons
	UPROPERTY()
	bool InAutoMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoMode")
	bool HasAutoMode;
	//1 = Very Fast, 250 Slow i.e. Sniper// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoMode", meta = (ClampMin = "1", ClampMax = "250", UIMin = "1", UIMax = "250"))
	float FireRate = 1;

	///Sounds///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* DryFireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* GunShotSound;

	///Animations///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UAnimMontage* FireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UAnimMontage* GunReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	FVector WeaponOffset;

	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> FireCameraShake;
	//Decal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	AActor* DecalBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	FVector DecalSize;
	//Impacts
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem*	DefaultImpactParticles;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem*	FleshImpactParticles;

	//Tracer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UParticleSystem* FireParticles;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracer")
	// AActor* TracerBP;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracer")
	// FVector TracerSize;

	// FLash is Disabled
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle")
	// float MuzzleFlashIntensity;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle")
	// class UMaterialInterface* MuzzleMaterial;

	//Linetraced
	// AActor* ActorHit;

	//Attachments?
	//class UStaticMeshComponent AttachmentSlot1; 
};
