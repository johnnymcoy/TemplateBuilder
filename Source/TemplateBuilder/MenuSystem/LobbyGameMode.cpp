// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "MenuGameInstance.h"
#include "TimerManager.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	NumberOfPlayers++;
	UE_LOG(LogTemp, Warning, TEXT("Number of Players: %d"), NumberOfPlayers);
	if(NumberOfPlayers >= 2)
	{
		GetWorldTimerManager().SetTimer(LoadMapTimer, this, &ALobbyGameMode::LoadMap, StartDelay, false);
	}
}

void ALobbyGameMode::LoadMap()
{
	bUseSeamlessTravel = true;
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	UMenuGameInstance* GameInstance = Cast<UMenuGameInstance>(GetGameInstance());
	if(GameInstance == nullptr) return;
	GameInstance->StartSession();
	World->ServerTravel("/Game/Demos/ShooterDemo/Levels/ShooterDemoLevel?listen");

}
void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	NumberOfPlayers--;
	UE_LOG(LogTemp, Warning, TEXT("Number of Players: %d"), NumberOfPlayers);
}

