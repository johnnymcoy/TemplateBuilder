// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "ShootingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATEBUILDER_API UShootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShootingComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Weapon Fire
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void ShootGun();
	void ValidateShootGun();
	void StopShootGun();
	

	//Weapon Swaps/Pickups
	void PickupWeapon();
	void EquipWeapon();
	void SwapWeapon();
	void HolsterWeapon();
	void ThrowWeapon();
	void DropWeapon();
	//ClearWeapon();

	
protected:
	bool PrimaryEquiped;
	bool bIsHolstered;


	virtual void BeginPlay() override;

	//Stats
	//less than 1 is Low, 5 Is Very high
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Weapon", meta=(UIMin = "0.1", UIMax = "10.0"))
	float Accuracy = 1;

private:
	FTimerHandle ShootingTimerHandle;
	
	//Already has weapon
	void AddAmmo();

	UAnimMontage* GetReloadAnimation();
	
	//When changed on Owner Activate Component
	UPROPERTY(VisibleAnywhere)
	bool bHasGun;

	//Weapon
	struct FWeaponData CurrentWeaponData;
	TArray<FWeaponData> CurrentWeapons;


};
