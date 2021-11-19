// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

USTRUCT()
struct FServerRowData
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
class TEMPLATEBUILDER_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* Interface);
	void Setup();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	virtual void SetServerList(TArray<FServerRowData> ServerInfo);
	
protected:
	IMenuInterface* MenuInterface;
};
