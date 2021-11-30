// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPawn.h"

AMenuPawn::AMenuPawn()
{

}

void AMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// PlayerInputComponent->BindAction("PauseAction", IE_Pressed, this, &AMenuPawn::ExitGame);


}

