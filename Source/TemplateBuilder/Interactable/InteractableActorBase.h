// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "InteractableActorBase.generated.h"

UCLASS()
class TEMPLATEBUILDER_API AInteractableActorBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor* Caller);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnPickUp(AActor* Caller);
	virtual void OnPickUp_Implementation(AActor* Caller);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StartFocus();
	virtual void StartFocus_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EndFocus();
	virtual void EndFocus_Implementation();


};
