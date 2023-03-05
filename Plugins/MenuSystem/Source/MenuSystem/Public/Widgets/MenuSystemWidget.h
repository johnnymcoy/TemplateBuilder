// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/MenuSystem.h"
#include "MenuSystemWidget.generated.h"

USTRUCT()
struct FServerRow
{
	GENERATED_BODY()

	FString ServerName;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
	uint32 Ping;
	//Location?
};


UCLASS()
class MENUSYSTEM_API UMenuSystemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetMenuInterface(IMenuSystem* Interface);
	void Setup();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	virtual void SetServerList(TArray<FServerRow> ServerInfo);
	virtual void SetPlayerIsDead(bool bIsPlayerDead);
	
protected:
	IMenuSystem* MenuInterface;
};
