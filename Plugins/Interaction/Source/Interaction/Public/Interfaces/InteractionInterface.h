// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE(MinimalAPI,meta = (CannotImplementInterfaceInBlueprint))
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class INTERACTION_API IInteractionInterface
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnInteract(AActor* Caller) = 0;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnPickUp(AActor* Caller) = 0;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void StartFocus() = 0;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void EndFocus() = 0;
public:
};
