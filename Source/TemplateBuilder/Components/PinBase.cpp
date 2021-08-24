// Fill out your copyright notice in the Description page of Project Settings.


#include "PinBase.h"
#include "Components/WidgetComponent.h"
#include "TemplateBuilder/Components/PinWidget.h"

APinBase::APinBase()
{
	PinWidgetComponent = CreateDefaultSubobject<UPinWidget>(TEXT("PinWidget"));
	PinWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PinWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	bReplicates = true;
}

void APinBase::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(PinData.Lifetime);
	PinData.Location = GetActorLocation();
	PinWidgetComponent->UpdatePinData(PinData);
}
