// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponent.h"

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

void UCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

