// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShootingComponent.h"
#include "Components/ActorComponent.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "ShootingReplicationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATEBUILDER_API UShootingReplicationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShootingReplicationComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerPickupWeapon(FWeaponData WeaponToPickup);
	UFUNCTION(Server, WithValidation, Reliable)
	void ServerValidateShootGun();
	
	FWeaponData GetCurrentWeaponData() const{return CurrentWeaponData;};

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(Replicated)
	FWeaponData CurrentWeaponData;
	UPROPERTY()
	class UShootingComponent* ShootingComponent;
};
