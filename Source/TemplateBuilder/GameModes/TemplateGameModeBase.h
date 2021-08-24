// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TemplateGameModeBase.generated.h"

UCLASS()
class TEMPLATEBUILDER_API ATemplateGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected: 

	virtual void BeginPlay() override;

public:
	
	virtual void PawnKilled(APawn* PawnKilled);

	virtual void EnemyAIKilled();

	virtual TSubclassOf<class UUserWidget> GetGameModeHUD();


private:

	//void EndGame(bool bIsPlayerWinner);

};
