// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"


UCLASS()
class WEAPONSYSTEM_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void SetCrosshairLocation();
	UFUNCTION(BlueprintCallable)
	void SetCrosshairVisibility(float Alpha);
	UFUNCTION(BlueprintCallable)
	void SetGunRecoil(float RecoilAmount);
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterpSpeed = 12.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UpperBound = -80.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LowerBound = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilMultiplier = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerSpeedMultiplier = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UpdateSpeed = 0.02f;


private:
	UPROPERTY(meta = (BindWidget))
	UWidget* CrosshairTop;
	UPROPERTY(meta = (BindWidget))
	UWidget* CrosshairBottom;
	UPROPERTY(meta = (BindWidget))
	UWidget* CrosshairLeft;
	UPROPERTY(meta = (BindWidget))
	UWidget* CrosshairRight;

	float Recoil;
	bool bRecoilFire;

	 float ProcessNumbers(class UCanvasPanelSlot* CanvasSlot, bool bVertical) const;

	
};
