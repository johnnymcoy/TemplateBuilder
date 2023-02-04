// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CustomCharacterBase.h"
#include "CustomPlayerCharacter.generated.h"

UCLASS()
class CUSTOMALS_API ACustomPlayerCharacter : public ACustomCharacterBase
{
	GENERATED_BODY()
public:
	ACustomPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
	// Parent Functions
	virtual void AimPressedAction() override;
	virtual void AimReleasedAction() override;

	


};
