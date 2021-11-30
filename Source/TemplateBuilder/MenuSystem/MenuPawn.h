// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MenuPawn.generated.h"

UCLASS()
class TEMPLATEBUILDER_API AMenuPawn : public APawn
{
	GENERATED_BODY()

public:
	AMenuPawn();

protected:
	virtual void BeginPlay() override;

public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

};
