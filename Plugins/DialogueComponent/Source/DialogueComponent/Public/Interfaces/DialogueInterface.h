// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialogueInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDialogueInterface : public UInterface
{
	GENERATED_BODY()
};

class DIALOGUECOMPONENT_API IDialogueInterface
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Dialogue)
	bool StartDialogue(class UDlgDialogue* Dialogue, const TArray<UObject*>& Participants);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Dialogue)
	void SelectDialogueOption(int32 Option);

};
