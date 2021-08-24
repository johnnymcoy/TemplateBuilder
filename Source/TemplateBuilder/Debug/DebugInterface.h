// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DebugInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDebugInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEMPLATEBUILDER_API IDebugInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debug")
	void EnableWeaponDebug(bool DebugStatus);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debug")
	void EnableControllerDebug(bool DebugStatus);





};
