// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class TEMPLATEBUILDER_API UMainMenuWidget : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenuWidget();

	virtual void SetServerList(TArray<FServerRowData> ServerInfo) override;

	void SelectIndex(uint32 Index);
	
protected:
	virtual bool Initialize() override;
	UFUNCTION()
	void StartScreenButtonClicked();
	UFUNCTION()
	void SinglePlayerButtonClicked();
	UFUNCTION()
	void OptionsButtonClicked();
	UFUNCTION()
	void CharacterButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();
	UFUNCTION()
	void SearchServersButtonClicked();
	UFUNCTION()
	void HostButtonClicked();
	UFUNCTION()
	void JoinMenuButtonClicked();
	UFUNCTION()
	void HostMenuButtonClicked();

	UFUNCTION()
	void BackButtonClicked();
	UFUNCTION()
	void QuitButtonClicked();

	void UpdateServerRows();
	UFUNCTION()
	void NumberOnSliderChanged(float Value);


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GameVersion = 0.11;
private:
	TSubclassOf<class UUserWidget>	ServerRowClass;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* StartScreenButton;

	
	UPROPERTY(meta = (BindWidget))
	class UButton* SinglePlayerButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* CharacterButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* OptionsButton;


	
	UPROPERTY(meta = (BindWidget))
	class UButton* SearchServersButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* HostMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinBackButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* HostBackButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* OptionsBackButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* CharacterBackButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IPInputTextBox;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerNameTextBox;

	UPROPERTY(meta = (BindWidget))
	class USlider* NumberOfPlayersSlider;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NumberOfPlayersText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameVersionText;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
	class UWidget* StartScreen;
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinIPMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* OptionsMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* CharacterMenu;



	UPROPERTY(meta = (BindWidget))
	class UWidget* SearchingProgressSpinner;

	float NumberOfPlayers;
	TOptional<uint32> SelectedIndex;
};

