// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DialogueComponent.h"
#include "UObject/Interface.h"
#include "CustomCharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCustomCharacter : public UInterface
{
	GENERATED_BODY()
};

class CUSTOMALS_API ICustomCharacter
{
	GENERATED_BODY()

public:
	
	// virtual void GetInteractionComponent() = 0;
	// virtual void GetShootingComponent() = 0;

	// Used to start Dialogue 
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	virtual void GetDialogueComponent(UDialogueComponent* &Out_DialogueComponent) const = 0;
	
};
