// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSCustomController.h"
#include "Blueprint/UserWidget.h"
#include "TemplateBuilder/GameModes/TemplateGameModeBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void AALSCustomController::BeginPlay() 
{
    Super::BeginPlay();
    // ATemplateGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ATemplateGameModeBase>();
    // if(GameMode)
    // {
    //     GameModeWBPClass = GameMode->GetGameModeHUD();
    //     if(GameModeWBPClass != nullptr)
    //     {
    //         GameModeRef = CreateWidget(this, GameModeWBPClass);
    //         GameModeRef->AddToViewport();
            //UpdateScore(KillScore);
            //UpdateRound(GameRound);
    //   }
    // }
}

void AALSCustomController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    if (bIsWinner) //All AI is Dead 
    {
        if(WinScreenClass != nullptr)
        {
            UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
            WinScreen->AddToViewport();
        }
    }
    else //If Player Dies Lose screen
    {
        if(GameOverScreenClass != nullptr)
        {
            UUserWidget* GameOverScreen = CreateWidget(this, GameOverScreenClass);
            GameOverScreen->AddToViewport();
        }
    }


}

void AALSCustomController::UpdateScore_Implementation(int Score) 
{
    KillScore = Score;
}

void AALSCustomController::UpdateRound_Implementation(int Round) 
{
	GameRound = Round;
}

void AALSCustomController::EnableControllerDebug_Implementation(bool DebugStatus) 
{
    bDebuggingMode = DebugStatus;
}

void AALSCustomController::Disable() 
{
    UE_LOG(LogTemp, Warning, TEXT("PlayerController Disable Function"));
    UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this);
    bShowMouseCursor = true;
    //SetInputMode(FInputModeUIOnly);
    // DisableInput(Cast<APlayerController>(this));
    // DisableInput(this);
}
