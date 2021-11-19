// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRowWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.h"

bool UServerRowWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;
	if (!ensure(ServerSelectButton != nullptr)) return false;    
	ServerSelectButton->OnClicked.AddDynamic(this, &UServerRowWidget::ServerSelectButtonClicked);
	DefaultColor = ServerSelectButton->ColorAndOpacity;
	return true;
}

void UServerRowWidget::Setup(UMainMenuWidget* Parent, uint32 Index)
{
	MainMenu = Parent;
	IndexNumber = Index;
}

void UServerRowWidget::SetAsSelected(bool bIsSelected)
{
	if(bIsSelected)
	{
		ServerSelectButton->SetColorAndOpacity(SelectedColor);
	}
	else
	{
		ServerSelectButton->SetColorAndOpacity(DefaultColor);
	}
}

void UServerRowWidget::ServerSelectButtonClicked()
{
	MainMenu->SelectIndex(IndexNumber);
}
