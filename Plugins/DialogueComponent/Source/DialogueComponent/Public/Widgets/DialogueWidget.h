// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

UCLASS(Blueprintable)
class DIALOGUECOMPONENT_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Dialogue", meta=(DisplayName = "Refresh"))
	void ReceiveRefresh(class UDlgContext* Context);

	void Refresh(class UDlgContext* Context);
	
	
};
