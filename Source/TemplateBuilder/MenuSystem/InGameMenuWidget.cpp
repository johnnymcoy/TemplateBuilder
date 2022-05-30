// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenuWidget.h"
#include "GameFramework/GameModeBase.h" 
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UInGameMenuWidget::Initialize() 
{
	bool Success = Super::Initialize();
	if(!Success) return false;
	if (!ensure(MainMenuButton != nullptr)) return false;
	if (!ensure(BackButton != nullptr)) return false;
	if (!ensure(OptionsButton != nullptr)) return false;
	if (!ensure(RestartLevelButton != nullptr)) return false;

	MainMenuButton->OnClicked.AddDynamic(this, &UInGameMenuWidget::MainMenuButtonClicked);
	BackButton->OnClicked.AddDynamic(this, &UInGameMenuWidget::BackButtonClicked);
	OptionsButton->OnClicked.AddDynamic(this, &UInGameMenuWidget::OptionsButtonClicked);
	RestartLevelButton->OnClicked.AddDynamic(this, &UInGameMenuWidget::RestartLevelButtonClicked);
	return true;
}

void UInGameMenuWidget::SetPlayerIsDead(bool bIsPlayerDead)
{
	bPlayerDead = bIsPlayerDead;
	BackButton->SetIsEnabled(!bPlayerDead);
};

void UInGameMenuWidget::MainMenuButtonClicked() 
{
	UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return;
	APlayerController* PlayerControllerReference = World->GetFirstPlayerController();
	if(!ensure(PlayerControllerReference != nullptr)) return;
	if(World->IsServer()) 
	{
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if(GameMode)
		{
			GameMode->ReturnToMainMenuHost();
		}
	} 
	else 
	{
		if (PlayerControllerReference) 
		{
			PlayerControllerReference->ClientReturnToMainMenuWithTextReason(FText::FromString("Back to main menu"));
		}
	}
}

void UInGameMenuWidget::BackButtonClicked() 
{
	RemoveFromViewport();
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerControllerReference = World->GetFirstPlayerController();
	if (!ensure(PlayerControllerReference != nullptr)) return;
	FInputModeGameOnly InputModeData;
	PlayerControllerReference->SetInputMode(InputModeData);
	PlayerControllerReference->bShowMouseCursor = false;
}

void UInGameMenuWidget::OptionsButtonClicked()
{
	//todo Options menu
}

void UInGameMenuWidget::RestartLevelButtonClicked()
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(),	true);
	UE_LOG(LogTemp,Warning,TEXT("%s Levelname "), *LevelName);
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), TRAVEL_Absolute);
}

