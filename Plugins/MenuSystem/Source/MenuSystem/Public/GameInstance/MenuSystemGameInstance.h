// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/MenuSystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "MenuSystemGameInstance.generated.h"

UCLASS()
class MENUSYSTEM_API UMenuSystemGameInstance : public UGameInstance, public IMenuSystem
{
	GENERATED_BODY()
public:
	UMenuSystemGameInstance(const FObjectInitializer & ObjectInitializer);
	
	virtual void Init() override;

	UFUNCTION(Exec, BlueprintCallable)
	void LoadMenu();
	UFUNCTION(Exec, BlueprintCallable)
	virtual void LaunchInGameMenu(bool bIsDead = false);

	virtual void Host(FOnlineSessionSettings HostSessionSettings) override;
	virtual void SinglePlayer() override;
	virtual void Character() override;
	virtual void Options() override;
	void StartSession();

	UFUNCTION(Exec)
	virtual void JoinManualAddress(const FString& IpAddress) override;
	
	UFUNCTION(Exec)
	virtual void Join(uint32 Index) override;

	
	UFUNCTION(BlueprintCallable, Exec, Category="Menu System")
	virtual void SearchServers() override;
	UFUNCTION(BlueprintCallable, Exec, Category="Menu System")
	virtual void Quit(APlayerController* Player) override;

	//? Temp from other project
	UFUNCTION(BlueprintCallable, Exec, Category="Menu System")
	void JoinFirstPossibleMatch();
	UFUNCTION(BlueprintCallable, Exec, Category="Menu System")
	void SessionFoundComplete(bool bSuccess);
	// UFUNCTION()
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	UFUNCTION(BlueprintCallable, Category="Menu System")
	FName GetCurrentSessionName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Menu System")
	FString StartingLevel = "/Game/Demos/ShooterDemo/Levels/ShooterDemoLevel?listen";
protected:
	
private:
	TSubclassOf<class UUserWidget>	MenuClass;
	TSubclassOf<class UUserWidget>	OptionsMenuClass;
	TSubclassOf<class UUserWidget>	InGameMenuClass;
	
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	FOnlineSessionSettings SessionSettings;
	
	UPROPERTY()
	class UMenuSystemWidget* MainMenu;
	UPROPERTY()
	class UOptionsMenuSystemWidget* OptionsMenu;
	

	void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);
	void OnFindSessionsComplete(bool bSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);
	void OnNetworkFailure(UWorld* World, UNetDriver* NewDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	void CreateSession();

	UPROPERTY(EditAnywhere, Category="Menu System")
	bool bDebuggingMode;

	FName CurrentSessionName;

};

