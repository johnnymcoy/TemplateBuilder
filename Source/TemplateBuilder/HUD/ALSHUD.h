// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ALSHUD.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBUILDER_API AALSHUD : public AHUD
{
	GENERATED_BODY()

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;


};
