// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuSystemLobbyGameMode.generated.h"

UCLASS()
class MENUSYSTEM_API AMenuSystemLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	

private:
	UPROPERTY(VisibleAnywhere)
	float StartDelay = 5;
	
	void LoadMap();

	
	uint32 NumberOfPlayers;
	FTimerHandle LoadMapTimerHandle;

};
