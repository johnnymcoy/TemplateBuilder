// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CompanionWidget.generated.h"


UCLASS()
class AITOOLKIT_API UCompanionWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category="Companion")
	void SetCompanionName(FName CompanionName);
	UFUNCTION(BlueprintCallable, Category="Companion")
	void SetInteractionPrompt(FString InteractionPrompt);
	
	// void UpdateWidget();
	
protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CompanionDisplayNameTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InteractionPromptTextBlock;

};
