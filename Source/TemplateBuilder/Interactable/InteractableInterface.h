// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UINTERFACE(MinimalAPI,meta = (CannotImplementInterfaceInBlueprint))
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};


class TEMPLATEBUILDER_API IInteractableInterface
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnInteract(AActor* Caller) = 0;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnPickUp(AActor* Caller) = 0;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void StartFocus() = 0;
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void EndFocus() = 0;

};
