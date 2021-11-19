// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "OnlineSessionSettings.h"
#include "ServerRowWidget.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

UMainMenuWidget::UMainMenuWidget()
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
	// if (!ensure(ServerRowReference->StaticClass() != nullptr)) return;
	// ServerRowClass = ServerRowReference->StaticClass();
}

bool UMainMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;
	if (!ensure(JoinButton != nullptr)) return false;
	if (!ensure(HostButton != nullptr)) return false;
	if (!ensure(JoinBackButton != nullptr)) return false;
	if (!ensure(HostBackButton != nullptr)) return false;
	if (!ensure(QuitButton != nullptr)) return false;
	if (!ensure(JoinMenuButton != nullptr)) return false;
	if (!ensure(SearchServersButton != nullptr)) return false;
	if (!ensure(SearchingProgressSpinner != nullptr)) return false;
	if (!ensure(NumberOfPlayersSlider != nullptr)) return false;
	if (!ensure(NumberOfPlayersText != nullptr)) return false;    
	HostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HostButtonClicked);
	JoinBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::BackButtonClicked);
	HostBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::BackButtonClicked);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenuWidget::JoinButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitButtonClicked);
	HostMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HostMenuButtonClicked);
	JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::JoinMenuButtonClicked);
	SearchServersButton->OnClicked.AddDynamic(this, &UMainMenuWidget::SearchServersButtonClicked);
	SearchingProgressSpinner->SetVisibility(ESlateVisibility::Hidden);
	NumberOfPlayersSlider->OnValueChanged.AddDynamic(this, &UMainMenuWidget::NumberOnSliderChanged);

	return true;
}

void UMainMenuWidget::JoinMenuButtonClicked() 
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	if (!ensure(SearchingProgressSpinner != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if(MenuInterface != nullptr)
	{
		MenuInterface->SearchServers();
		SearchingProgressSpinner->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainMenuWidget::HostMenuButtonClicked()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(HostMenu);

}


void UMainMenuWidget::HostButtonClicked() 
{
    if (!ensure(ServerNameTextBox != nullptr)) return;
    if(MenuInterface != nullptr)
    {
        FOnlineSessionSettings SessionSettings;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.bUsesPresence = true;
        SessionSettings.NumPublicConnections = NumberOfPlayers;
        // SessionSettings.bAllowJoinInProgress = true;
        // SessionSettings.bAntiCheatProtected = true;
        // SessionSettings.bIsLANMatch = true;
        // SessionSettings.Settings
        FString ServerName = ServerNameTextBox->GetText().ToString();
        SessionSettings.Set(TEXT("Server Name"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
        MenuInterface->Host(SessionSettings);
    }
}

void UMainMenuWidget::SetServerList(TArray<FServerRowData> ServerInfo)
{
    UWorld* WorldRef = GetWorld();
    if (!ensure(WorldRef != nullptr)) return;
    ServerList->ClearChildren();
    uint32 i = 0;
    for (const FServerRowData& ServerData : ServerInfo)
    {
        UServerRowWidget* Row = CreateWidget<UServerRowWidget>(WorldRef, ServerRowClass);
        Row->ServerNameText->SetText(FText::FromString(ServerData.ServerName));
        FString PlayerCount = FString::Printf(TEXT("(%d/%d)"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
        Row->ServerPlayerCountText->SetText(FText::FromString(PlayerCount));
        Row->ServerOwningPlayerText->SetText(FText::FromString(ServerData.HostUsername));
        Row->ServerPingText->SetText(FText::FromString(FString::Printf(TEXT("%dms"), ServerData.Ping)));

        Row->Setup(this, i);
        ++i;
        ServerList->AddChild(Row);
    }
    SearchingProgressSpinner->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuWidget::SelectIndex(uint32 Index)
{
    SelectedIndex = Index;
    UpdateServerRows();
}

void UMainMenuWidget::UpdateServerRows()
{
    for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
    {
        UServerRowWidget* Row = Cast<UServerRowWidget>(ServerList->GetChildAt(i));
        if(Row != nullptr)
        {
            bool bIsSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
            Row->SetAsSelected(bIsSelected);
        }
    }
}

void UMainMenuWidget::NumberOnSliderChanged(float Value)
{
    NumberOfPlayers = Value;
    NumberOfPlayersText->SetText(FText::AsNumber(Value));
}

void UMainMenuWidget::JoinButtonClicked() 
{
    if(SelectedIndex.IsSet() && MenuInterface != nullptr)
    {
        MenuInterface->Join(SelectedIndex.GetValue());
        UE_LOG(LogTemp, Warning, TEXT("Selected Index: %d"), SelectedIndex.GetValue());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Nothing Selected"));
    }
    //TODO: Create manual IP input
    // if(MenuInterface != nullptr)
    // {
    //     if (!ensure(IPInputTextBox != nullptr)) return;
    //     FString IpAddressInput = IPInputTextBox->GetText().ToString();
    //     MenuInterface->JoinManualAddress(IpAddressInput);
    // }
}

void UMainMenuWidget::SearchServersButtonClicked()
{
    if (!ensure(SearchingProgressSpinner != nullptr)) return;
    if(MenuInterface != nullptr)
    {
        MenuInterface->SearchServers();
        SearchingProgressSpinner->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMainMenuWidget::BackButtonClicked() 
{
    if (!ensure(MenuSwitcher != nullptr)) return;
    if (!ensure(MainMenu != nullptr)) return;
    MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenuWidget::QuitButtonClicked() 
{
    if(MenuInterface != nullptr)
    {
        MenuInterface->Quit();
    }
}
