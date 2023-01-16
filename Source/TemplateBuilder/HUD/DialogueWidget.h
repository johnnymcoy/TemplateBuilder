// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBUILDER_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void Refresh(class UDlgContext* Context);
	
	
};
