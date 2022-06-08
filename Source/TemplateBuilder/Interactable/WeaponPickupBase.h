// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "WeaponPickupBase.generated.h"

UCLASS()
class TEMPLATEBUILDER_API AWeaponPickupBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AWeaponPickupBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	FWeaponData WeaponPickupData;

		
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void WeaponDroppedEvent(FWeaponData DroppedWeaponData);

	// Might Not need for pickup weapons

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnInteract(AActor* Caller) override;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "Receive Interact"))
	void ReceiveOnInteract(AActor* Caller);
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnPickUp(AActor* Caller) override;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "On Pickup"))
	void ReceiveOnPickUp(AActor* Caller);
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void StartFocus() override;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "Start Focus"))
	void ReceiveStartFocus();
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void EndFocus() override;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "End Focus"))
	void ReceiveEndFocus();

protected:
	virtual void BeginPlay() override;

private:
	void SetMesh();

};
