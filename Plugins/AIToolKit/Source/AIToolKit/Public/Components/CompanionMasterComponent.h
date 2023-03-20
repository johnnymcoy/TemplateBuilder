// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterComponent.h"
#include "Interfaces/CompanionInterface.h"
#include "CompanionMasterComponent.generated.h"

UCLASS()
class AITOOLKIT_API UCompanionMasterComponent : public UCharacterComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Companion|Master")
	void CreateCompanionWidget();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Companions")
	bool bCommandingCompanion;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Companions")
	AActor* CompanionToCommand;
private:


};
