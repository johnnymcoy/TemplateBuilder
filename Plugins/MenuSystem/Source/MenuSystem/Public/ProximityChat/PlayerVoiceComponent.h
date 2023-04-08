// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "PlayerVoiceComponent.generated.h"

UCLASS()
class MENUSYSTEM_API UPlayerVoiceComponent : public UAudioComponent
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category="Proximity Chat")
	void PlayVoiceChat(const TArray<uint8>& VoiceData, int32 SampleRate);
};
