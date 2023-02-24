// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Weapon.h"
#include "WeaponFramework.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAmmoChanged, float, Health, float, MaxHealth,const class UDamageType*, DamageType);


// USTRUCT()
// struct FHitScanTraceResult
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY()
// 	int a;
// 	// UPROPERTY()
// 	// TEnumAsByte<EPhysicalSurface> SurfaceType;
// 	UPROPERTY()
// 	FVector_NetQuantize TraceTo;
// };


UCLASS()
class WEAPONSYSTEM_API AWeaponFramework : public AActor, public IWeapon
{
	GENERATED_BODY()
	
public:	
	AWeaponFramework();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debugging")
	bool bDebuggingMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debugging")
	bool bInfiniteAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debugging")
	float DrawDebugTime = 2.0f;

	// UPROPERTY(BlueprintAssignable, Category = "Ammo")
	// FOnAmmoChanged OnAmmoChanged;


protected:
	virtual void BeginPlay() override;

	//- Gun Visual Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMeshComponent; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USceneComponent* Muzzle;

	//- Interface Functions
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy) override;
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector in_Location,const FRotator in_Rotation, const AActor* ActorToIgnore, const float in_Accuracy);

	virtual void Reload(float ReloadTime) override;
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void CancelReload() override;
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void SwitchAutoMode() override;
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void BlindFireWeapon() override;
	virtual void MoveUMG(bool bIsRightShoulder) override;
	virtual void FadeInUMG(bool bIsAiming) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual FWeaponData_T GetWeaponData() override {return WeaponData;};
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual bool IsInAutoMode() override {return WeaponData.bIsInAutoMode;};
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual bool IsClipEmpty() override {return WeaponData.CurrentAmmo <= 0;};
	
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void SetWeaponData(const FWeaponData_T in_WeaponData) override {WeaponData = in_WeaponData;};
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Weapons")
	virtual void SetWeaponMesh(USkeletalMesh* SkeletalMesh) override;
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastSetWeaponMesh(USkeletalMesh* SkeletalMesh);
	
	//- Blueprint Functions
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon", meta=(DisplayName = "Fire"))
	void ReceiveFire(FHitResult Hit, APawn* InstigatorPawn);


	//- Multiplayer //
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShoot();

	UFUNCTION(Server, Reliable)
	void ServerSetReloading(bool bReloading);
	UFUNCTION(Server, Reliable)
	void ServerReload();

	// ? Temp Add to private later
	// UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	// FWeaponData_T WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	FWeaponStats_T WeaponStats;

private:


	bool LineTrace(FHitResult& Hit, FVector& ShotDirection, FLinearColor Color, FVector CustomLineEnd = FVector::ZeroVector);
	void ApplyDamageToActor(const FHitResult& Hit, FVector ShotDirection);
	void CalculateBulletSpread(FVector& NewBulletSpread);
	

	FWeaponData_T WeaponData;
	// FWeaponStats_T WeaponStats;

	//todo add to proper location
	float GunRange = 200000.0f;

	
	//Server Playing the Effects
	// UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	// FHitScanTraceResult HitScanTrace;
	// UFUNCTION()
	// void OnRep_HitScanTrace();


	// Bullet Fire Data - turn to struct? Replicate?
	
	// todo remove replicated.. Maybe bDebugging? 
	UPROPERTY()
	FVector TraceLocation;
	FRotator TraceRotation;
	float AccuracyMultiplier;
	UPROPERTY()
	const AActor* TraceActorToIgnore;

	
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;


	FTimerHandle ReloadTimerHandle;
	bool bIsBeingReloaded;
	bool CanShoot();
	

};





