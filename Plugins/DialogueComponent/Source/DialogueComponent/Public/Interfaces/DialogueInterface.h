// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialogueInterface.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UDialogueInterface : public UInterface
{
	GENERATED_BODY()
};

class DIALOGUECOMPONENT_API IDialogueInterface
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	virtual bool StartDialogue(class UDlgDialogue* Dialogue, const TArray<UObject*>& Participants) = 0;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	virtual void SelectDialogueOption(int32 Option) = 0;


};
