// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DialogueWidget.h"

void UDialogueWidget::Refresh(class UDlgContext* Context)
{
	if(Context == nullptr)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
		ReceiveRefresh(Context);
	}
}

