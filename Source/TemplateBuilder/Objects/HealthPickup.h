// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TemplateBuilder/Interactable/InteractableActorBase.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBUILDER_API AHealthPickup : public AInteractableActorBase
{
	GENERATED_BODY()


public:
	AHealthPickup();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FX")
	class UParticleSystem* PickupEffects;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	TSubclassOf<UDamageType> DamageType;

protected:
	virtual void OnInteract(AActor* Caller);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthToGain = 20;
	
private:
	void PlayParticleEffects();
};
