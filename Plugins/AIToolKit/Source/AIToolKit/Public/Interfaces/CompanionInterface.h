// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CompanionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCompanionInterface : public UInterface
{
	GENERATED_BODY()
};

class AITOOLKIT_API ICompanionInterface
{
	GENERATED_BODY()

public:
	
	//-			Commands		//
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void SetMaster(AActor* MasterActor) = 0;
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void Follow() = 0;
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void Fetch(UPrimitiveComponent* ComponentToGrab) = 0;
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void StartCombat(AActor* TargetEnemy) = 0;
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void Wait(FVector TargetLocation) = 0;
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void GrabObject() = 0;

	
	// todo Move to Character Interface in Custom ALS
	//-			AI Control		//
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void Attack() = 0;

};
