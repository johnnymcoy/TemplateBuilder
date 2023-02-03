// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CustomCharacterBase.h"
#include "CustomAICharacter.generated.h"

UCLASS()
class CUSTOMALS_API ACustomAICharacter : public ACustomCharacterBase
{
	GENERATED_BODY()
	
public:
	ACustomAICharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;


private:
	
};
