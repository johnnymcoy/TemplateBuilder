// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "InteractableActor.generated.h"

UCLASS()
class INTERACTION_API AInteractableActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnInteract(AActor* Caller) override;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction", meta=(DisplayName = "On Receive Interaction"))
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

};
