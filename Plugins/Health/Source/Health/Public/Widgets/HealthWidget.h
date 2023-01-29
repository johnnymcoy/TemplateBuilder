// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

UCLASS(Blueprintable)
class HEALTH_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetHasShield(bool in_bHasShield);

	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float CurrentHealth, float MaxHealth);
	UFUNCTION(BlueprintCallable)
	void UpdateHealthAndShield(float CurrentHealth, float MaxHealth, float CurrentShieldHealth, float MaxShieldHealth, TSubclassOf<class UDamageType> DamageType = nullptr);


protected:
	virtual bool Initialize() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* ShieldBar;
private:
	UPROPERTY()
	bool bHasShield;
};

