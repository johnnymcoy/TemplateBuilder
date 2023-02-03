// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Weapon.h"
#include "WeaponFramework.generated.h"

UCLASS()
class WEAPONSYSTEM_API AWeaponFramework : public AActor, public IWeapon
{
	GENERATED_BODY()
	
public:	
	AWeaponFramework();

protected:
	virtual void BeginPlay() override;

	//Gun Visual Components//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMeshComponent; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USceneComponent* Muzzle;

	// Interface Functions
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

	virtual void SetWeaponData(const FWeaponData_T in_WeaponData) override {WeaponData = in_WeaponData;};
	
	// Blueprint Functions
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon", meta=(DisplayName = "Fire"))
	void ReceiveFire(FHitResult Hit, APawn* InstigatorPawn);


	//Multiplayer//
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShoot();
	// UFUNCTION(Server, Reliable)
	// void ServerSetReloading(bool bReloading);
	// UFUNCTION(Server, Reliable)
	// void ServerReload();

	
private:

	bool LineTrace(FHitResult& Hit, FVector& ShotDirection);
	
	FWeaponData_T WeaponData;
	FWeaponStats_T WeaponStats;

	// Bullet Fire Data - turn to struct? Replicate?
	// UPROPERTY(Replicated)
	FVector TraceLocation;
	FRotator TraceRotation;
	float AccuracyMultiplier;
	UPROPERTY()
	const AActor* TraceActorToIgnore;

	
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;


	
	bool bIsBeingReloaded;
	bool CanShoot();
	

};

