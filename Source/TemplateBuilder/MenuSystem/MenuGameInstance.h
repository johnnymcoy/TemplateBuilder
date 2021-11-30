// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuGameInstance.generated.h"


UCLASS()
class TEMPLATEBUILDER_API UMenuGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UMenuGameInstance(const FObjectInitializer & ObjectInitializer);
	
	virtual void Init() override;

	UFUNCTION(Exec, BlueprintCallable)
	void LoadMenu();
	UFUNCTION(Exec, BlueprintCallable)
	virtual void LaunchInGameMenu();
	
	virtual void Host(FOnlineSessionSettings HostSessionSettings) override;
	virtual void SinglePlayer() override;
	virtual void Character() override;
	virtual void Options() override;
	void StartSession();

	UFUNCTION(Exec)
	virtual void JoinManualAddress(const FString& IpAddress) override;
	UFUNCTION(Exec)
	virtual void Join(uint32 Index) override;
	UFUNCTION(Exec)
	virtual void SearchServers() override;
	UFUNCTION(Exec)
	virtual void Quit() override;


	private:
	TSubclassOf<class UUserWidget>	MenuClass;
	TSubclassOf<class UUserWidget>	InGameMenuClass;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	class UMenuWidget* MainMenu;
	
	FOnlineSessionSettings SessionSettings;

	void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);
	void OnFindSessionsComplete(bool bSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);
	void OnNetworkFailure(UWorld* World, UNetDriver* NewDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	void CreateSession();

};