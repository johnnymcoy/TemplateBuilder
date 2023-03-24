// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AICharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAICharacter : public UInterface
{
	GENERATED_BODY()
};

class AITOOLKIT_API IAICharacter
{
	GENERATED_BODY()

public:

	// virtual 
	// virtual bool bIsCompanion() = 0;
};
