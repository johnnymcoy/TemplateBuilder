// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ServerRowMenuSystemWidget.h"
#include "Components/Button.h"
#include "Widgets/MainMenuSystemWidget.h"

bool UServerRowMenuSystemWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;
	if (!ensure(ServerSelectButton != nullptr)) return false;    
	ServerSelectButton->OnClicked.AddDynamic(this, &UServerRowMenuSystemWidget::ServerSelectButtonClicked);
	DefaultColor = ServerSelectButton->ColorAndOpacity;
	return true;
}

void UServerRowMenuSystemWidget::Setup(UMainMenuSystemWidget* Parent, uint32 Index)
{
	MainMenu = Parent;
	IndexNumber = Index;
}

void UServerRowMenuSystemWidget::SetAsSelected(bool bIsSelected)
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

void UServerRowMenuSystemWidget::ServerSelectButtonClicked()
{
	MainMenu->SelectIndex(IndexNumber);
}

