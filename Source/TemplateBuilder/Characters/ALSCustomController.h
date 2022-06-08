// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ALSPlayerController.h"
#include "TemplateBuilder/Debug/DebugInterface.h"
#include "ALSCustomController.generated.h"

UCLASS()
class TEMPLATEBUILDER_API AALSCustomController : public AALSPlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	
	UFUNCTION(BlueprintNativeEvent, Blueprintable)
	void UpdateScore(int Score);
	virtual void UpdateScore_Implementation(int Score);

	UFUNCTION(BlueprintNativeEvent, Blueprintable)
	void UpdateRound(int Round);
	virtual void UpdateRound_Implementation(int Round);

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* GameModeRef;

	//Debug Interface
	// virtual void EnableControllerDebug(bool DebugStatus) override;
	void Disable();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> GameOverScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;

	TSubclassOf<class UUserWidget> GameModeWBPClass;


	int KillScore = 0;
	int GameRound = 1;

	bool bDebuggingMode;

};
