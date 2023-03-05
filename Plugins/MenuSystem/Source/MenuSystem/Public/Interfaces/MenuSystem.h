// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OnlineSessionSettings.h"
#include "MenuSystem.generated.h"

UINTERFACE(MinimalAPI)
class UMenuSystem : public UInterface
{
	GENERATED_BODY()
};

class MENUSYSTEM_API IMenuSystem
{
	GENERATED_BODY()

public:
	virtual void Host(FOnlineSessionSettings HostSessionSettings) = 0;
	virtual void SinglePlayer() = 0;
	virtual void Character() = 0;
	virtual void Options() = 0;

	virtual void Join(uint32 Index) = 0;
	virtual void JoinManualAddress(const FString& IpAddress) = 0;
	virtual void Quit() = 0;
	virtual void SearchServers() = 0;

};
