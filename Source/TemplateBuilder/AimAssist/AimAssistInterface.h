// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AimAssistInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAimAssistInterface : public UInterface
{
	GENERATED_BODY()
};

class TEMPLATEBUILDER_API IAimAssistInterface
{
	GENERATED_BODY()

public:
	virtual bool bIsDead() = 0;
};
