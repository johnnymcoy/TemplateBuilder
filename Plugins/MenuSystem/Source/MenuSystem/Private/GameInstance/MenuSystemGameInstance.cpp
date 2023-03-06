// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MenuSystemGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "Engine/Engine.h"
#include "Online.h"
#include "Widgets/MainMenuSystemWidget.h"
#include "Widgets/OptionsMenuSystemWidget.h"


const static FName SESSION_NAME = NAME_GameSession;

UMenuSystemGameInstance::UMenuSystemGameInstance(const FObjectInitializer& ObjectInitializer)
{
	// Todo All the BP references 
	// ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	// if (!ensure(MainMenuBPClass.Class != nullptr)) return;
	// MenuClass = MainMenuBPClass.Class;
	// ConstructorHelpers::FClassFinder<UUserWidget> OptionsMenuBPClass(TEXT("/Game/MenuSystem/WBP_OptionsMenu"));
	// if (!ensure(OptionsMenuBPClass.Class != nullptr)) return;
	// OptionsMenuClass = OptionsMenuBPClass.Class;
	// ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	// if (!ensure(InGameMenuBPClass.Class != nullptr)) return;
	// InGameMenuClass = InGameMenuBPClass.Class;
}

void UMenuSystemGameInstance::Init()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystemFound %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMenuSystemGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMenuSystemGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMenuSystemGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMenuSystemGameInstance::OnJoinSessionComplete);
		}
	}
	if(GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UMenuSystemGameInstance::OnNetworkFailure);
	}
}

void UMenuSystemGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;
	MainMenu = CreateWidget<UMenuSystemWidget>(GetFirstLocalPlayerController(), MenuClass);
	if (!ensure(MainMenu != nullptr)) return;
	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UMenuSystemGameInstance::CreateSession()
{
	if(SessionInterface.IsValid())
	{
		//If we're not on steam, set to lan match
		SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UMenuSystemGameInstance::StartSession()
{
	if(SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UMenuSystemGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
	if(!bSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Create Session"));
		return;
	}
	UEngine* EngineReference = GetEngine();
	if (!ensure(EngineReference != nullptr)) return;
	// EngineReference->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Hosting...%ls"), *SessionSettings.Get(TEXT("Server Name"))));
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	World->ServerTravel("/Game/MenuSystem/Lobby?listen");
}

void UMenuSystemGameInstance::Host(FOnlineSessionSettings HostSessionSettings)
{
	SessionSettings = HostSessionSettings;
	if(SessionInterface.IsValid())
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if(ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UMenuSystemGameInstance::SearchServers()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if(SessionSearch.IsValid())
	{
		// SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("Searching For sessions..."));
	}
}

void UMenuSystemGameInstance::OnFindSessionsComplete(bool bSuccess)
{
	if(bSuccess && SessionSearch.IsValid() && MainMenu != nullptr)
	{
		UE_LOG(LogTemp,Warning, TEXT("Find Session Complete"));
		// for (auto& It = SessionSearch->SearchResults.CreateIterator(); It; It++)
		TArray<FServerRow> ServerInfo;
		FServerRow TestData;
		for (auto& It : SessionSearch->SearchResults)
		{
			FServerRow ServerData;
			ServerData.MaxPlayers = It.Session.SessionSettings.NumPublicConnections;
			ServerData.CurrentPlayers = (ServerData.MaxPlayers - It.Session.NumOpenPublicConnections);
			ServerData.HostUsername = It.Session.OwningUserName;
			ServerData.Ping = It.PingInMs;
			FString ServerName;
			if(It.Session.SessionSettings.Get(TEXT("Server Name"), ServerName))
			{
				ServerData.ServerName = ServerName;
			}
			else
			{
				ServerData.ServerName = "Error in Server Name";
			}
			UE_LOG(LogTemp,Warning, TEXT("Session Found: %s"), *ServerData.ServerName);
			ServerInfo.Add(ServerData);
		}
		MainMenu->SetServerList(ServerInfo);
	}
	else
	{
		UE_LOG(LogTemp,Warning, TEXT("Find Session Failed"));
	}
}

void UMenuSystemGameInstance::JoinManualAddress(const FString& IpAddress)
{
	UEngine* EngineReference = GetEngine();
	if (!ensure(EngineReference != nullptr)) return;
	// EngineReference->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, (TEXT("Joining  ") + IpAddress) );
	if(IpAddress.IsEmpty())
	{
		EngineReference->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Enter IP Address")));
	}
	else
	{
		EngineReference->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Joining %s"), *IpAddress));
		APlayerController* PlayerControllerReference = GetFirstLocalPlayerController();
		if (!ensure(PlayerControllerReference != nullptr)) return;
		PlayerControllerReference->ClientTravel(IpAddress, ETravelType::TRAVEL_Absolute);
	}
}

void UMenuSystemGameInstance::Join(uint32 Index)
{
	if(!SessionInterface.IsValid()) return;
	if(!SessionSearch.IsValid()) return;
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UMenuSystemGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	if(!SessionInterface.IsValid()) return;
	FString Address;
	if(!SessionInterface->GetResolvedConnectString(SessionName,Address))
	{
		UE_LOG(LogTemp, Error, TEXT("Could not get Connect String (JoinSessionComplete)"));
		return;
	}
	UEngine* EngineReference = GetEngine();
	if (!ensure(EngineReference != nullptr)) return;
	APlayerController* PlayerControllerReference = GetFirstLocalPlayerController();
	if (!ensure(PlayerControllerReference != nullptr)) return;
    
	EngineReference->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
	PlayerControllerReference->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UMenuSystemGameInstance::LaunchInGameMenu(bool bIsDead)
{
	if (!ensure(InGameMenuClass != nullptr)) return;
	UMenuSystemWidget* InGameMenu = CreateWidget<UMenuSystemWidget>(GetFirstLocalPlayerController(), InGameMenuClass);
	if (!ensure(InGameMenu != nullptr)) return;
	//todo: if(multiplayer Game)
	//UGameplayStaticics::PauseGame
	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
	InGameMenu->SetPlayerIsDead(bIsDead);
}

void UMenuSystemGameInstance::SinglePlayer()
{
	// TODO Level Name
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Demos/ShooterDemo/Levels/ShooterDemoLevel"), TRAVEL_Absolute);
}

void UMenuSystemGameInstance::Character()
{
	UE_LOG(LogTemp,Warning, TEXT("CHARACTER MENU"));
}

void UMenuSystemGameInstance::Options()
{
	if (!ensure(OptionsMenuClass != nullptr)) return;
	OptionsMenu = CreateWidget<UOptionsMenuSystemWidget>(GetFirstLocalPlayerController(), OptionsMenuClass);
	if (!ensure(OptionsMenu != nullptr)) return;
	OptionsMenu->Setup();
	// APlayerController* PlayerControllerReference = GetFirstLocalPlayerController();
	// if (!ensure(PlayerControllerReference != nullptr)) return;
	// PlayerControllerReference->ConsoleCommand(("Quit"), true);
}

void UMenuSystemGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
	if(bSuccess)
	{
		CreateSession();
	}
}

void UMenuSystemGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NewDriver, ENetworkFailure::Type FailureType,
	const FString& ErrorString)
{
	LoadMenu();
}

void UMenuSystemGameInstance::Quit()
{
	APlayerController* PlayerControllerReference = GetFirstLocalPlayerController();
	if (!ensure(PlayerControllerReference != nullptr)) return;
	PlayerControllerReference->ConsoleCommand(("Quit"), true);
}
