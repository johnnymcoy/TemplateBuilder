// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ALSCustomCharacter.h"
#include "TemplateBuilder/Interactable/InteractableInterface.h"
#include "ALSCustomAICharacter.generated.h"

UCLASS()
class TEMPLATEBUILDER_API AALSCustomAICharacter : public AALSCustomCharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AALSCustomAICharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Health Bar
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HealthBar;


public:	

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

	virtual void Death_Implementation() override;
	UFUNCTION(NetMulticast , Reliable)
	void MulticastDeath();
	bool DeathOnce = false;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TraceForward();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PickupNearbyWeapon();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float PickupReach;
private:
	UFUNCTION()    
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void FaceHealthBarToPlayer();

	FTimerHandle HealthBarTimer;
	bool bHealthBarActive;

	
};
