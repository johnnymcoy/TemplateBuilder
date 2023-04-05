// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/MenuSystemWidget.h"
#include "MainMenuSystemWidget.generated.h"

UCLASS()
class MENUSYSTEM_API UMainMenuSystemWidget : public UMenuSystemWidget
{
	GENERATED_BODY()


public:
	UMainMenuSystemWidget();
	virtual void SetServerList(TArray<FServerRow> ServerInfo) override;


	void SelectIndex(uint32 Index);
	


protected:
	virtual bool Initialize() override;
	UFUNCTION(BlueprintCallable, Category="Menu System")
	void StartScreenButtonClicked();

	UFUNCTION(BlueprintCallable, Category="Menu System")
	void JoinButtonClicked();
	UFUNCTION(BlueprintCallable, Category="Menu System")
	void SearchServersButtonClicked();
	UFUNCTION(BlueprintCallable, Category="Menu System")
	void HostButtonClicked();

	UFUNCTION(BlueprintCallable, Category="Menu System")
	void QuitGame();

	void UpdateServerRows();
	UFUNCTION()
	void NumberOnSliderChanged(float Value);

	UFUNCTION(BlueprintCallable, Category="Menu System")
	void SetIsControllerInput(bool bIsController);
	UPROPERTY(BlueprintReadWrite, Category="Menu System")
	int32 ButtonFocused = 1;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MenuPageIndex = 0;
	UPROPERTY(BlueprintReadWrite)
	int32 CampaignPageIndex = 0;
	UPROPERTY(BlueprintReadWrite)
	int32 MultiplayerPageIndex = 0;
	


	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* StartScreenButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* JoinButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;
	

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidget* StartScreen;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidget* MainMenu;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	bool bIsControllerInput;

	
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


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* GameVersionText;

	
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
	class UPanelWidget* ServerList;




	UPROPERTY(meta = (BindWidget))
	class UWidget* SearchingProgressSpinner;

	float NumberOfPlayers;
	TOptional<uint32> SelectedIndex;

};
