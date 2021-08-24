// Fill out your copyright notice in the Description page of Project Settings.


#include "DismembermentComponent.h"

// Sets default values for this component's properties
UDismembermentComponent::UDismembermentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UDismembermentComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UDismembermentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

