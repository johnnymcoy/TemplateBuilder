// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDamageType.h"
#include "StandardBulletDamageType.generated.h"

UCLASS()
class TEMPLATEBUILDER_API UStandardBulletDamageType : public UBaseDamageType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float GunImpulse;
	
};
