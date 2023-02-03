// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "WeaponPickup.generated.h"

UCLASS()
class WEAPONSYSTEM_API AWeaponPickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnInteract(AActor* Caller);
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnPickUp(AActor* Caller);
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void StartFocus();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void EndFocus();

};
