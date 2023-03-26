// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MenuSystemPlayerController.h"

void AMenuSystemPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	
	
}
