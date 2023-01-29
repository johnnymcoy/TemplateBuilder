// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthBarWidgetComponent.h"

#include "Widgets/HealthWidget.h"

UHealthBarWidgetComponent::UHealthBarWidgetComponent()
{
	// BUG Possible - causes editor to throw constant errors about "other object in slot" 
	// SetDrawAtDesiredSize(true);
	// WidgetClass = UHealthWidget::StaticClass();
}

void UHealthBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void UHealthBarWidgetComponent::UpdateHeathAndShield(float CurrentHealth, float MaxHealth, float CurrentShield,
	float MaxShield)
{
	UHealthWidget* HealthWidget = Cast<UHealthWidget>(GetUserWidgetObject());
	if(HealthWidget != nullptr)
	{
		HealthWidget->UpdateHealthAndShield(CurrentHealth, MaxHealth, CurrentShield,MaxShield);
	}
}
