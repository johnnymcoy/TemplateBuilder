// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

UCLASS()
class TEMPLATEBUILDER_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	AExplosiveBarrel();

protected:
	virtual void BeginPlay() override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class URadialForceComponent* RadialForceComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	class UParticleSystem*	ExplosionParticles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	float ExplosionImpulse;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	float ExplosionScale;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	class UMaterialInterface* BarrelExplodedMaterial;


private:
	
	UFUNCTION()
	void OnHealthChanged(class UHealthComponent* HealthComp, float Health, float DefaultHealth, float Shield, float DefaultShield, const class UDamageType* DamageType);

	UPROPERTY(ReplicatedUsing=OnRep_Exploded)
	bool bDied;

	UFUNCTION()
	void OnRep_Exploded();

};
