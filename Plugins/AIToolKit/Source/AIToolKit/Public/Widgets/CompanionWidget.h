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

	UFUNCTION(BlueprintCallable, Category="Companion")
	void SetIsCommanding(bool bCommanding);

	UFUNCTION(BlueprintImplementableEvent, Category="Companion")
	void OnCommandingChanged();
	
	// void UpdateWidget();
	
protected:
	virtual bool Initialize() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Companion")
	bool bIsCommanding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CompanionDisplayNameTextBlock;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* InteractionPromptTextBlock;
private:

};
