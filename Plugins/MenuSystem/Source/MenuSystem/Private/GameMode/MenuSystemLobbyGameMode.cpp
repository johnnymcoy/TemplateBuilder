// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MenuSystemLobbyGameMode.h"
#include "GameInstance/MenuSystemGameInstance.h"
#include "TimerManager.h"


void AMenuSystemLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	NumberOfPlayers++;
	UE_LOG(LogTemp, Warning, TEXT("Number of Players: %d"), NumberOfPlayers);
	if(NumberOfPlayers >= 2)
	{
		GetWorldTimerManager().SetTimer(LoadMapTimerHandle, this, &AMenuSystemLobbyGameMode::LoadMap, StartDelay, false);
	}

}

void AMenuSystemLobbyGameMode::LoadMap()
{
	bUseSeamlessTravel = true;
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	UMenuSystemGameInstance* GameInstance = Cast<UMenuSystemGameInstance>(GetGameInstance());
	if(GameInstance == nullptr) return;
	GameInstance->StartSession();
	// Todo: Put the Starting Level somewhere else..  Maybe the game Instance
	
	World->ServerTravel("/Game/Demos/ShooterDemo/Levels/ShooterDemoLevel?listen");
}


void AMenuSystemLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	NumberOfPlayers--;
	UE_LOG(LogTemp, Warning, TEXT("Number of Players: %d"), NumberOfPlayers);
}
