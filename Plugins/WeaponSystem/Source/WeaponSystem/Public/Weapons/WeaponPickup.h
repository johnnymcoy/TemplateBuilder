// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponStructs.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "WeaponPickup.generated.h"

UCLASS()
class WEAPONSYSTEM_API AWeaponPickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AWeaponPickup();
	
	//- Visual Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USkeletalMeshComponent* GunMesh;
	
	//- Weapon Data
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	FWeaponData_T WeaponPickupData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	//-	For when weapon is spawned	//
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void WeaponDroppedEvent(FWeaponData_T DroppedWeaponData);
	// UFUNCTION(Server, Unreliable,  Category = "Interaction")
	// void Server_WeaponDroppedEvent(FWeaponData_T DroppedWeaponData);


	
	// Interface Functions
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnInteract(AActor* Caller) override;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnPickUp(AActor* Caller) override;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void StartFocus() override;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void EndFocus() override;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual FString GetObjectDisplayName() override {return DisplayName;}


	// Blueprint Calls
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "On Interacted With"))
	void ReceiveOnInteract(AActor* Caller);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "On Pickup"))
	void ReceiveOnPickUp(AActor* Caller);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "Start Focus"))
	void ReceiveStartFocus();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "End Focus"))
	void ReceiveEndFocus();
	
protected:
	virtual void BeginPlay() override;
private:
	void SetMesh();


};
