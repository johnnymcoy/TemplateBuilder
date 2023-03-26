// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CompanionWidget.h"
#include "Components/TextBlock.h"



bool UCompanionWidget::Initialize()
{
	const bool Success = Super::Initialize();
	if(!Success) return false;
	if (!ensure(CompanionDisplayNameTextBlock != nullptr)) return false;
	return true;
}

void UCompanionWidget::SetCompanionName(FName CompanionName)
{
	CompanionDisplayNameTextBlock->SetText(FText::FromName(CompanionName));
}

void UCompanionWidget::SetInteractionPrompt(FString InteractionPrompt)
{
	//	Todo: Add global? or just in this 
	FString InteractionKey = "[E] ";
	const FText FormattedPrompt = FText::FromString(InteractionKey.Append(InteractionPrompt));
	InteractionPromptTextBlock->SetText(FormattedPrompt);
}

void UCompanionWidget::SetIsCommanding(bool bCommanding)
{
	bIsCommanding = bCommanding;
	OnCommandingChanged();
}

