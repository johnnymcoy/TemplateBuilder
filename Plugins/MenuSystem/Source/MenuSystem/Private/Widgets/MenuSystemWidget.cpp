// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MenuSystemWidget.h"

void UMenuSystemWidget::SetMenuInterface(IMenuSystem* Interface)
{
	MenuInterface = Interface;
}

void UMenuSystemWidget::Setup()
{
	AddToViewport();
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerControllerReference = World->GetFirstPlayerController();
	if (!ensure(PlayerControllerReference != nullptr)) return;
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerControllerReference->SetInputMode(InputModeData);
	PlayerControllerReference->bShowMouseCursor = true;
}

void UMenuSystemWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromViewport();
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerControllerReference = World->GetFirstPlayerController();
	if (!ensure(PlayerControllerReference != nullptr)) return;
	FInputModeGameOnly InputModeData;
	PlayerControllerReference->SetInputMode(InputModeData);
	PlayerControllerReference->bShowMouseCursor = false;
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMenuSystemWidget::SetServerList(TArray<FServerRow> ServerInfo)
{
	
}

void UMenuSystemWidget::SetPlayerIsDead(bool bIsPlayerDead)
{
	
}
