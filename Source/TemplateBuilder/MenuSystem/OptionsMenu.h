// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsMenu.generated.h"

UCLASS()
class TEMPLATEBUILDER_API UOptionsMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup();

protected:
	virtual bool Initialize() override;

	
private:

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;
	UFUNCTION()
    void Exit();

	UFUNCTION()
	void ChangeResolution480();
	UFUNCTION()
	void ChangeResolution720();
	UFUNCTION()
	void ChangeResolution1080();
	UFUNCTION()
	void ChangeResolution1440();
	
	//Screen Resolution
	UPROPERTY(meta = (BindWidget))
	class UButton* Res480Button;
	UPROPERTY(meta = (BindWidget))
	class UButton* Res720Button;
	UPROPERTY(meta = (BindWidget))
	class UButton* Res1080Button;
	UPROPERTY(meta = (BindWidget))
	class UButton* Res1440Button;

	UFUNCTION()
	void ChangeViewDistanceNear();
	UFUNCTION()
	void ChangeViewDistanceMedium();
	UFUNCTION()
	void ChangeViewDistanceFar();

	//View Distance (0.4,0.7,1)
	UPROPERTY(meta = (BindWidget))
	class UButton* ViewNearButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ViewMediumButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ViewFarButton;

	UFUNCTION()
	void ChangeAALow();
	UFUNCTION()
	void ChangeAAMedium();
	UFUNCTION()
	void ChangeAAHigh();
	UFUNCTION()
	void ChangeAAEpic();
	
	//Anti Aliasing (0,2,4,6)
	UPROPERTY(meta = (BindWidget))
	class UButton* AALowButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* AAMediumButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* AAHighButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* AAEpicButton;

	UFUNCTION()
	void ChangeShadowLow();
	UFUNCTION()
	void ChangeShadowMedium();
	UFUNCTION()
	void ChangeShadowHigh();
	UFUNCTION()
	void ChangeShadowEpic();
	
	//Shadow Quality (0,1,2,3)
	UPROPERTY(meta = (BindWidget))
	class UButton* ShadowLowButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ShadowMediumButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ShadowHighButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ShadowEpicButton;

	UFUNCTION()
	void ChangeTextureLow();
	UFUNCTION()
	void ChangeTextureMedium();
	UFUNCTION()
	void ChangeTextureHigh();
	UFUNCTION()
	void ChangeTextureEpic();
	
	//Texture Quality (0,1,2,3)
	UPROPERTY(meta = (BindWidget))
	class UButton* TQLowButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* TQMediumButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* TQHighButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* TQEpicButton;

};
