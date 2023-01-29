// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HealthWidget.h"
#include "Components/ProgressBar.h"



bool UHealthWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;
	if (!ensure(HealthBar != nullptr)) return false;
	if (!ensure(ShieldBar != nullptr)) return false;
	return true;
}


void UHealthWidget::SetHasShield(bool in_bHasShield)
{
	bHasShield = in_bHasShield;
	if(!bHasShield)
	{
		ShieldBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHealthWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	HealthBar->SetPercent(CurrentHealth/MaxHealth);
}

void UHealthWidget::UpdateHealthAndShield(float CurrentHealth, float MaxHealth, float CurrentShieldHealth, float MaxShieldHealth,
                                 TSubclassOf<UDamageType> DamageType)
{
	HealthBar->SetPercent(CurrentHealth/MaxHealth);
	ShieldBar->SetPercent(CurrentShieldHealth/MaxShieldHealth);
}
