// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponent.h"
#include "Components/Widget.h"

UCharacterComponent::UCharacterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCharacterComponent::SetController(AController* Controller)
{
	OwnerController = Controller;
	if(bIsNPC)
	{
		OwnerAIController = Cast<AAIController>(Controller);
	}
	else
	{
		OwnerPlayerController = Cast<APlayerController>(Controller);
	}
}

void UCharacterComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCharacterComponent::SetInputModeGameAndUI(bool bGameAndUI,UWidget* InWidgetToFocus, bool bShowMouse)
{
	if(bGameAndUI)
	{
		FInputModeGameAndUI InputModeData;
		OwnerPlayerController->SetInputMode(InputModeData);
		OwnerPlayerController->bShowMouseCursor = bShowMouse;
		if (InWidgetToFocus != nullptr)
		{
			InputModeData.SetWidgetToFocus(InWidgetToFocus->TakeWidget());
		}
	}
	else
	{
		FInputModeGameOnly InputModeData;
		OwnerPlayerController->SetInputMode(InputModeData);
		OwnerPlayerController->bShowMouseCursor = bShowMouse;
	}
}

void UCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

