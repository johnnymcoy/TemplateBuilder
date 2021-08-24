// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieHordeGameMode.h"
#include "GameFramework/Controller.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "TemplateBuilder/AI/AIControllerBase.h"
#include "TemplateBuilder/Characters/ALSCustomController.h"


void AZombieHordeGameMode::BeginPlay() 
{
    Super::BeginPlay();
    PlayerController = Cast<AALSCustomController>(UGameplayStatics::GetPlayerController(this, 0));
}




void AZombieHordeGameMode::PawnKilled(APawn* PawnKilled) 
{
    Super::PawnKilled(PawnKilled);


    APlayerController* PlayerControllerKilled = Cast<APlayerController>(PawnKilled->GetController());
    if(PlayerControllerKilled !=nullptr)
    {
        EndGame(false);
    }
    //Score = Score++;
    for(AAIControllerBase* AIController : TActorRange<AAIControllerBase>(GetWorld()))
    {
        if(!AIController->IsDead()) // If any of the controllers are Not dead
        {
            return;
        }
    }
    EndGame(true);

}

void AZombieHordeGameMode::EnemyAIKilled() 
{
    Score++;
    if(PlayerController)
    {
        PlayerController->UpdateScore(Score);
        if(Score / (Round*Round) == 1)
        {
            Round++;
            PlayerController->UpdateRound(Round);
            if(Round > 19)
            EndGame(true);
        }
    }
}

TSubclassOf<class UUserWidget> AZombieHordeGameMode::GetGameModeHUD() 
{
    return RoundHUDClass;
}

void AZombieHordeGameMode::EndGame(bool bIsPlayerWinner) 
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4);
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool BIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), BIsWinner);
    }
    // if(bIsPlayerWinner)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("You Win!"));
    //     UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4);
    // }
    // else
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("Game Over"));
    // }
    
}

