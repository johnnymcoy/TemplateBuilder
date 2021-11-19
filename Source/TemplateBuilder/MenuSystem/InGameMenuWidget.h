// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenuWidget.generated.h"

UCLASS()
class TEMPLATEBUILDER_API UInGameMenuWidget : public UMenuWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void MainMenuButtonClicked();
	UFUNCTION()
	void BackButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;
};
