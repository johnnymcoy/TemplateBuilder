// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProximityChatBPLib.generated.h"

/**
 * 
 */
UCLASS()
class MENUSYSTEM_API UProximityChatBPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category="Proximity Chat")
	static float CalculateVoiceAttenuation(const FVector& SpeakerLocation, const FVector& ListenerLocation, float MaxDistance, float VolumeAtMaxDistance);
	
};
