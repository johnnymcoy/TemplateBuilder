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

	//Bool Can be picked up?
	// UFUNCTION(Server, Reliable)
	// void ServerInteract(AActor* Caller);
	// UFUNCTION(NetMulticast, Reliable)
	// void MulticastInteract(AActor* Caller);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void WeaponDroppedEvent(FWeaponData DroppedWeaponData);

	// Might Not need for pickup weapons

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor* Caller) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnPickUp(AActor* Caller);
	virtual void OnPickUp_Implementation(AActor* Caller) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StartFocus();
	virtual void StartFocus_Implementation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EndFocus();
	virtual void EndFocus_Implementation() override;

protected:
	virtual void BeginPlay() override;


private:
	void SetMesh();
	UPROPERTY(ReplicatedUsing=OnRep_PickedUp)
	bool bPickedup;
	UFUNCTION()
	void OnRep_PickedUp();
	UFUNCTION(Server, Reliable)
	void ClearMesh();

	

};
