// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthBarWidgetComponent.h"

UHealthBarWidgetComponent::UHealthBarWidgetComponent()
{
	SetDrawAtDesiredSize(true);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
