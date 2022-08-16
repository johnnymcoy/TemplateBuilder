// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TemplateBuilder/Weapons/WeaponClassIdentifier.h"
#include "ALSV4_CPP/Public/Library/ALSCharacterEnumLibrary.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "WeaponInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TemplateBuilder/Debug/DebugInterface.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WeaponBase.generated.h"

//Weapon Interface 

// Contatins infromation of a single Hitscan Weapon Linetrace
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

	UPROPERTY()
	int a;
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;
	UPROPERTY()
	FVector_NetQuantize TraceTo;
};

//TO USE COMPLEX COLLISION MUST BE SET TO SIMPLE 
USTRUCT(BlueprintType)
struct FImpactEffect
{
	GENERATED_USTRUCT_BODY();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Dirt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* General01;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* General02;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Grass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Gravel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Ice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Leaves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Mud;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Sand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* SnowHeavy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* SnowLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Sparks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* SpecialAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* WaterHeavy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* WaterLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Carpet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Concrete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Metal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Plastic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Wood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impact")
	class UParticleSystem* Flesh;
};


UCLASS()
class TEMPLATEBUILDER_API AWeaponBase : public AWeaponClassIdentifier, public IWeaponInterface, public IDebugInterface
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	//Gun Visual Components//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USkeletalMeshComponent* GunMeshComponent; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USceneComponent* Muzzle;

protected:
	virtual void BeginPlay() override;

	void PlayFireEffects(FVector TraceEnd);
	//todo change to footstep way of getting surface
	// void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);
	void PlayImpactEffects(int SurfaceType, FVector ImpactPoint);

	//Server Playing the Effects
	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;
	UFUNCTION()
	void OnRep_HitScanTrace();

	//Ammo UMG Counter 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USceneComponent* UMG_RightLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USceneComponent* UMG_LeftLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UWidgetComponent* AmmoWidgetComponent;

	//todo Add attachments and locations for each 
	// Attachments
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* FlashlightAttachment; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* AttachmentSlot01; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* AttachmentSlot02; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* AttachmentSlot03; 
	
	
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	// class UUserWidget* WeaponWidget;

public:	
	UFUNCTION(BlueprintCallable, Category = "Weapon Functions")
	virtual void Fire() override;
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon Functions", meta=(DisplayName = "Fire"))
	void ReceiveFire(FHitResult Hit, APawn* InstigatorPawn);

	virtual void Reload(float ReloadTime) override;
	UFUNCTION(BlueprintCallable, Category = "Weapon Functions")
	virtual void CancelReload() override;
	UFUNCTION(BlueprintCallable, Category = "Weapon Functions")
	virtual void SwitchAutoMode() override;
	UFUNCTION(BlueprintCallable, Category = "Weapon Functions")
	virtual void BlindFireWeapon() override;
	virtual FWeaponData GetWeaponData() override {return GunWeaponData;};
	virtual bool IsInAutoMode() override {return GunWeaponData.bIsInAutoMode;};
	virtual void SetWeaponData(const FWeaponData in_WeaponData) override {GunWeaponData = in_WeaponData;};
	virtual void MoveUMG(bool bIsRightShoulder) override;
	virtual void FadeInUMG(bool bIsAiming) override;
	
	///Interface Functions
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void GetTraceParams(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy) override;

	UFUNCTION(Server, Reliable)
	void ServerGetTraceParams(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy);
	//Extra Functions
	virtual void ToggleFlashlight() override;


	//Multiplayer//
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShoot();
	UFUNCTION(Server, Reliable)
	void ServerSetReloading(bool bReloading);
	UFUNCTION(Server, Reliable)
	void ServerReload();
	

	//Debug Interface
	virtual void EnableWeaponDebug(bool DebugStatus) override {bDebuggingMode = DebugStatus;};


private:
	void Shoot();
	UFUNCTION()
	bool LineTrace(FHitResult& Hit, FVector& ShotDirection);
	void CalculateBulletSpread(FVector& NewBulletSpread);

	
	void FadeInUMGTimed(float Alpha);
	void FadeUMGIn();
	void FadeUMGOut();
	float UMGAlpha;

	FTimerHandle UMGTimer;

	bool CanShoot();
	bool bIsReloading;
	FTimerHandle ReloadTimer;
	float GunRange = 200000.0f;
	UPROPERTY(Replicated)
	FVector TraceLocation;
	FRotator TraceRotation;
	float AccuracyMultiplier;
	const class AActor* TraceActorToIgnore;
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	UParticleSystem* ParticleSelector(const int in_SurfaceType);

	//Muzzle Flash//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// class UStaticMeshComponent* MuzzleFlash;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	// class UPointLightComponent* PointLight;

	///////Debug///////
	bool bDebuggingMode;
	EDrawDebugTrace::Type DrawDebugType;
	void ApplyDamageToActor(const FHitResult& Hit, FVector ShotDirection);

public:
	//New Var for All Weapon Stats
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	FWeaponData GunWeaponData;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	FWeaponStats GunWeaponStats;

	//IN NEW STRUCT -> WEAPON STATS
	//todo Check All values are working
	//Base Variables//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	// float BulletSpread;
	//Weapon Damage 
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	// float DefaultDamage;

	float Damage;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	// float HeadMultiplier;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	// float GunImpulse;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	// TSubclassOf<class UDamageType> DamageType;

	//WEAPON DATA
	//Ammo
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	// int CurrentAmmoCount;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	// int TotalAmmoCount;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	// int ClipSize;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	// int MaxAmmo;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	// float Recoil;

	
	//for Automatic Weapons
	// UPROPERTY()
	// bool InAutoMode;
	
	//todo remove
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoMode")
	// bool HasAutoMode;
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
	FImpactEffect ImpactEffects;
	
	//Put Materials here //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<class UPhysicalMaterial*> Surfaces;

	//todo Remove
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
};
