// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InGameMenuSystemWidget.h"
#include "GameFramework/GameModeBase.h" 
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UInGameMenuSystemWidget::Initialize()
{
	const bool Success = Super::Initialize();
	if(!Success) return false;
	if (!ensure(MainMenuButton != nullptr)) return false;
	if (!ensure(BackButton != nullptr)) return false;
	if (!ensure(OptionsButton != nullptr)) return false;
	if (!ensure(RestartLevelButton != nullptr)) return false;

	MainMenuButton->OnClicked.AddDynamic(this, &UInGameMenuSystemWidget::MainMenuButtonClicked);
	BackButton->OnClicked.AddDynamic(this, &UInGameMenuSystemWidget::BackButtonClicked);
	OptionsButton->OnClicked.AddDynamic(this, &UInGameMenuSystemWidget::OptionsButtonClicked);
	RestartLevelButton->OnClicked.AddDynamic(this, &UInGameMenuSystemWidget::RestartLevelButtonClicked);
	return true;
}

void UInGameMenuSystemWidget::MainMenuButtonClicked()
{
	const UWorld* World = GetWorld();
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

void UInGameMenuSystemWidget::BackButtonClicked()
{
	RemoveFromViewport();
	const UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerControllerReference = World->GetFirstPlayerController();
	if (!ensure(PlayerControllerReference != nullptr)) return;
	FInputModeGameOnly InputModeData;
	PlayerControllerReference->SetInputMode(InputModeData);
	PlayerControllerReference->bShowMouseCursor = false;
}

void UInGameMenuSystemWidget::OptionsButtonClicked()
{
	//todo Options menu
}

void UInGameMenuSystemWidget::RestartLevelButtonClicked()
{
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(),true);
	UE_LOG(LogTemp,Warning,TEXT("%s Levelname "), *LevelName);
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), TRAVEL_Absolute);
}

void UInGameMenuSystemWidget::SetPlayerIsDead(bool bIsPlayerDead)
{
	bPlayerDead = bIsPlayerDead;
	BackButton->SetIsEnabled(!bPlayerDead);
	Super::SetPlayerIsDead(bIsPlayerDead);
}
