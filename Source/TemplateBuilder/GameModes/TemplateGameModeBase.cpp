// Fill out your copyright notice in the Description page of Project Settings.


#include "TemplateGameModeBase.h"


void ATemplateGameModeBase::BeginPlay() 
{
    Super::BeginPlay();
}

void ATemplateGameModeBase::PawnKilled(APawn* PawnKilled) 
{
    
}

void ATemplateGameModeBase::EnemyAIKilled() 
{
    
}

TSubclassOf<class UUserWidget> ATemplateGameModeBase::GetGameModeHUD() 
{
    return nullptr;
}


// void ATemplateGameModeBase::EndGame(bool bIsPlayerWinner) 
// {
    
// }