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
	void JoinButtonClicked();
	UFUNCTION()
	void SearchServersButtonClicked();
	UFUNCTION()
	void HostButtonClicked();

	void UpdateServerRows();
	UFUNCTION()
	void NumberOnSliderChanged(float Value);


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GameVersion = 0.11;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* StartScreenButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* JoinButton;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;
    UPROPERTY(BlueprintReadWrite)
    int32 MenuPageIndex = 0;
    UPROPERTY(BlueprintReadWrite)
    int32 CampainPageIndex = 0;
    UPROPERTY(BlueprintReadWrite)
    int32 MultiplayerPageIndex = 0;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidget* StartScreen;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidget* MainMenu;
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// class UWidget* JoinMenu;
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// class UWidget* JoinIPMenu;
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// class UWidget* HostMenu;
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// class UWidget* OptionsMenu;
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// class UWidget* CharacterMenu;


private:
	TSubclassOf<class UUserWidget>	ServerRowClass;
	


	
	UPROPERTY(meta = (BindWidget))
	class UButton* SearchServersButton;

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
	class UWidget* SearchingProgressSpinner;

	float NumberOfPlayers;
    TOptional<uint32> SelectedIndex;

};

