// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OptionsMenuSystemWidget.h"
#include "Components/Button.h"

void UOptionsMenuSystemWidget::Setup()
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


bool UOptionsMenuSystemWidget::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;
	if (!ensure(Res480Button != nullptr)) return false;
	if (!ensure(Res720Button != nullptr)) return false;
	if (!ensure(Res1080Button != nullptr)) return false;
	if (!ensure(Res1440Button != nullptr)) return false;
	if (!ensure(ViewNearButton != nullptr)) return false;
	if (!ensure(ViewMediumButton != nullptr)) return false;
	if (!ensure(ViewFarButton != nullptr)) return false;
	if (!ensure(AALowButton != nullptr)) return false;
	if (!ensure(AAMediumButton != nullptr)) return false;
	if (!ensure(AAHighButton != nullptr)) return false;
	if (!ensure(AAEpicButton != nullptr)) return false;
	if (!ensure(ShadowLowButton != nullptr)) return false;
	if (!ensure(ShadowMediumButton != nullptr)) return false;
	if (!ensure(ShadowHighButton != nullptr)) return false;
	if (!ensure(ShadowEpicButton != nullptr)) return false;
	if (!ensure(TQLowButton != nullptr)) return false;
	if (!ensure(TQMediumButton != nullptr)) return false;
	if (!ensure(TQHighButton != nullptr)) return false;
	if (!ensure(TQEpicButton != nullptr)) return false;

	Res480Button->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeResolution480);
	Res720Button->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeResolution720);
	Res1080Button->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeResolution1080);
	Res1440Button->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeResolution1440);
	ViewNearButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeViewDistanceNear);
	ViewMediumButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeViewDistanceMedium);
	ViewFarButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeViewDistanceFar);
	AALowButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeAALow);
	AAMediumButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeAAMedium);
	AAHighButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeAAHigh);
	AAEpicButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeAAEpic);
	ShadowLowButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeShadowLow);
	ShadowMediumButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeShadowMedium);
	ShadowHighButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeShadowHigh);
	ShadowEpicButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeShadowEpic);
	TQLowButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeTextureLow);
	TQMediumButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeTextureMedium);
	TQHighButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeTextureHigh);
	TQEpicButton->OnClicked.AddDynamic(this, &UOptionsMenuSystemWidget::ChangeTextureEpic);
	// BackButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::Exit);
	
	return true;
}

// void UOptionsMenu::Exit()
// {
// 	RemoveFromViewport();
// 	RemoveFromParent();
// 	Destruct();
// }

void UOptionsMenuSystemWidget::ChangeResolution480()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.setres 720x480"));
}
void UOptionsMenuSystemWidget::ChangeResolution720()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.setres 1280x720"));
}
void UOptionsMenuSystemWidget::ChangeResolution1080()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.setres 1920x1080"));
}
void UOptionsMenuSystemWidget::ChangeResolution1440()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.setres 2560x1440"));
}

void UOptionsMenuSystemWidget::ChangeViewDistanceNear()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.ViewDistanceScale 0.4"));
}

void UOptionsMenuSystemWidget::ChangeViewDistanceMedium()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.ViewDistanceScale 0.7"));
}

void UOptionsMenuSystemWidget::ChangeViewDistanceFar()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.ViewDistanceScale 1"));
}

void UOptionsMenuSystemWidget::ChangeAALow()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.PostProcessAAQuality 0"));
}

void UOptionsMenuSystemWidget::ChangeAAMedium()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.PostProcessAAQuality 2"));
}

void UOptionsMenuSystemWidget::ChangeAAHigh()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.PostProcessAAQuality 4"));
}

void UOptionsMenuSystemWidget::ChangeAAEpic()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.PostProcessAAQuality 6"));
}

void UOptionsMenuSystemWidget::ChangeShadowLow()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.ShadowQuality 0"));
}

void UOptionsMenuSystemWidget::ChangeShadowMedium()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.ShadowQuality 1"));
}

void UOptionsMenuSystemWidget::ChangeShadowHigh()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.ShadowQuality 2"));
}

void UOptionsMenuSystemWidget::ChangeShadowEpic()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.ShadowQuality 3"));
}

void UOptionsMenuSystemWidget::ChangeTextureLow()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.TextureQuality 0"));
}

void UOptionsMenuSystemWidget::ChangeTextureMedium()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.TextureQuality 1"));
}

void UOptionsMenuSystemWidget::ChangeTextureHigh()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.TextureQuality 2"));
}

void UOptionsMenuSystemWidget::ChangeTextureEpic()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.TextureQuality 3"));
}