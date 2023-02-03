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
	
	// Interface Functions
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnInteract(AActor* Caller) override;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnPickUp(AActor* Caller) override;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void StartFocus() override;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void EndFocus() override;

	// Blueprint Calls
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "On Interacted With"))
	void ReceiveOnInteract(AActor* Caller);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "On Pickup"))
	void ReceiveOnPickUp(AActor* Caller);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "Start Focus"))
	void ReceiveStartFocus();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "End Focus"))
	void ReceiveEndFocus();


};
