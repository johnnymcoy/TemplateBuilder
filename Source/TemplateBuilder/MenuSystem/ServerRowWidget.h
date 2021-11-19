// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRowWidget.generated.h"


UCLASS()
class TEMPLATEBUILDER_API UServerRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerPlayerCountText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerOwningPlayerText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerPingText;



	void Setup(class UMainMenuWidget* Parent, uint32 Index);

	void SetAsSelected(bool bIsSelected);
	
	UFUNCTION()
	void ServerSelectButtonClicked();
	
protected:
	virtual bool Initialize() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor SelectedColor;
	private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ServerSelectButton;
	UPROPERTY()
	class UMainMenuWidget* MainMenu;
	uint32 IndexNumber;
	FLinearColor DefaultColor;

};
