// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TemplateGameModeBase.h"
#include "ZombieHordeGameMode.generated.h"

UCLASS()
class TEMPLATEBUILDER_API AZombieHordeGameMode : public ATemplateGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void PawnKilled(APawn* PawnKilled) override;

	virtual void EnemyAIKilled() override;

	virtual TSubclassOf<class UUserWidget> GetGameModeHUD() override;
	UPROPERTY(BlueprintReadOnly)
	int Round = 1;
private:

	virtual void EndGame(bool bIsPlayerWinner);

	int Score = 0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> RoundHUDClass;

	class AALSCustomController* PlayerController;


	
	
};
