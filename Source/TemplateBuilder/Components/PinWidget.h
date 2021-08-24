// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TemplateBuilder/Components/PinBase.h"
#include "PinWidget.generated.h"

UCLASS()
class TEMPLATEBUILDER_API UPinWidget : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Pin")
	void UpdatePinData(FPinInfo PinData);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPinInfo WidgetPinData;
};
