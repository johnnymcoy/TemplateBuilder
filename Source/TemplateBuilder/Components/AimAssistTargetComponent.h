// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "TemplateBuilder/AimAssist/AimAssistInterface.h"
#include "AimAssistTargetComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class TEMPLATEBUILDER_API UAimAssistTargetComponent : public UCapsuleComponent, public IAimAssistInterface
{
	GENERATED_BODY()
public:
	virtual bool bIsDead() override{return bDead;};
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bDead;
};
