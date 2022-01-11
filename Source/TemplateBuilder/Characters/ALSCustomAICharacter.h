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

	virtual void Death_Implementation() override;
	UFUNCTION(NetMulticast , Reliable)
	void MulticastDeath();
	bool DeathOnce = false;



private:
	UFUNCTION()    
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void FaceHealthBarToPlayer();

	FTimerHandle HealthBarTimer;

	
};
