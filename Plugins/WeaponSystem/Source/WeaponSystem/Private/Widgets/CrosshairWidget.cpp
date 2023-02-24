// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CrosshairWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h" 
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


void UCrosshairWidget::NativeOnInitialized()
{
	UKismetSystemLibrary::K2_SetTimer(this, FString("SetCrosshairLocation"),UpdateSpeed,true);
}

void UCrosshairWidget::SetCrosshairLocation()
{
	if(GetOwningPlayerPawn() == nullptr){UE_LOG(LogTemp,Warning,TEXT("Error getting Player Pawn - CrosshairWidget"));return;}

	// FVector PlayerVelocity = PlayerPawn->GetVelocity();
	// float PlayerSpeed = (PlayerVelocity.Size() * -1.0f);
	UCanvasPanelSlot* CrosshairTopSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CrosshairTop);
	UCanvasPanelSlot* CrosshairBottomSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CrosshairBottom);
	UCanvasPanelSlot* CrosshairLeftSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CrosshairLeft);
	UCanvasPanelSlot* CrosshairRightSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CrosshairRight);

	// float CrosshairTopY = CrosshairTopSlot->GetPosition().Y;
	// const float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	// float ClampValue = UKismetMathLibrary::FInterpTo(CrosshairTopY, PlayerSpeed, DeltaSeconds, InterpSpeed);
	// float ClampResult = UKismetMathLibrary::FClamp(ClampValue, UpperBound, LowerBound);
	
	CrosshairTopSlot->SetPosition(FVector2D(0,ProcessNumbers(CrosshairTopSlot, true)));
	CrosshairBottomSlot->SetPosition(FVector2D(0,(ProcessNumbers(CrosshairTopSlot, true) * -1.0f)));
	CrosshairLeftSlot->SetPosition(FVector2D(ProcessNumbers(CrosshairLeftSlot, false),0));
	CrosshairRightSlot->SetPosition(FVector2D((ProcessNumbers(CrosshairLeftSlot, false) * -1.0f), 0));
	Recoil = 0;
	bRecoilFire = false;
}

void UCrosshairWidget::SetCrosshairVisibility(float Alpha)
{
	float Opacity = UKismetMathLibrary::Lerp(0, 1, Alpha);
	CrosshairTop->SetRenderOpacity(Opacity);
	CrosshairBottom->SetRenderOpacity(Opacity);
	CrosshairLeft->SetRenderOpacity(Opacity);
	CrosshairRight->SetRenderOpacity(Opacity);
}

void UCrosshairWidget::SetGunRecoil(float RecoilAmount)
{
	Recoil = (RecoilAmount * RecoilMultiplier);
	bRecoilFire = true;
}

float UCrosshairWidget::ProcessNumbers(UCanvasPanelSlot* CanvasSlot, bool bVertical) const
{
	const FVector PlayerVelocity = GetOwningPlayerPawn()->GetVelocity();
	const float PlayerSpeed = ((PlayerVelocity.Size() * -1.0f) * PlayerSpeedMultiplier);
	const float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	
	float Target = PlayerSpeed;
	if(bRecoilFire)
	{
		Target = PlayerSpeed + (Recoil * -1.0f);	
	}
	float Current = CanvasSlot->GetPosition().X;
	if(bVertical)
	{
		Current = CanvasSlot->GetPosition().Y;
	}
	const float ClampValue = UKismetMathLibrary::FInterpTo(Current, Target, DeltaSeconds, InterpSpeed);
	return UKismetMathLibrary::FClamp(ClampValue, UpperBound, LowerBound);
}
