// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class ECompanionState : uint8
{
	Wait,
	Follow,
	Combat,
	Fetch,
	
};



class AITOOLKIT_API CompanionDataTypes
{
public:
	CompanionDataTypes();
	~CompanionDataTypes();
};
