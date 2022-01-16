// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenu.h"
#include "Components/Button.h"

void UOptionsMenu::Setup()
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


bool UOptionsMenu::Initialize()
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

	Res480Button->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeResolution480);
	Res720Button->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeResolution720);
	Res1080Button->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeResolution1080);
	Res1440Button->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeResolution1440);
	ViewNearButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeViewDistanceNear);
	ViewMediumButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeViewDistanceMedium);
	ViewFarButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeViewDistanceFar);
	AALowButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeAALow);
	AAMediumButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeAAMedium);
	AAHighButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeAAHigh);
	AAEpicButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeAAEpic);
	ShadowLowButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeShadowLow);
	ShadowMediumButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeShadowMedium);
	ShadowHighButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeShadowHigh);
	ShadowEpicButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeShadowEpic);
	TQLowButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeTextureLow);
	TQMediumButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeTextureMedium);
	TQHighButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeTextureHigh);
	TQEpicButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::ChangeTextureEpic);
	BackButton->OnClicked.AddDynamic(this, &UOptionsMenu::UOptionsMenu::Exit);
	
	return true;
}

void UOptionsMenu::Exit()
{
	RemoveFromViewport();
	RemoveFromParent();
	Destruct();
}

void UOptionsMenu::ChangeResolution480()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.setres 720x480"));
}
void UOptionsMenu::ChangeResolution720()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.setres 1280x720"));
}
void UOptionsMenu::ChangeResolution1080()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.setres 1920x1080"));
}
void UOptionsMenu::ChangeResolution1440()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.setres 2560x1440"));
}

void UOptionsMenu::ChangeViewDistanceNear()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.ViewDistanceScale 0.4"));
}

void UOptionsMenu::ChangeViewDistanceMedium()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.ViewDistanceScale 0.7"));
}

void UOptionsMenu::ChangeViewDistanceFar()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.ViewDistanceScale 1"));
}

void UOptionsMenu::ChangeAALow()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.PostProcessAAQuality 0"));
}

void UOptionsMenu::ChangeAAMedium()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.PostProcessAAQuality 2"));
}

void UOptionsMenu::ChangeAAHigh()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.PostProcessAAQuality 4"));
}

void UOptionsMenu::ChangeAAEpic()
{
	GetWorld()->Exec(GetWorld(), TEXT("r.PostProcessAAQuality 6"));
}

void UOptionsMenu::ChangeShadowLow()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.ShadowQuality 0"));
}

void UOptionsMenu::ChangeShadowMedium()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.ShadowQuality 1"));
}

void UOptionsMenu::ChangeShadowHigh()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.ShadowQuality 2"));
}

void UOptionsMenu::ChangeShadowEpic()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.ShadowQuality 3"));
}

void UOptionsMenu::ChangeTextureLow()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.TextureQuality 0"));
}

void UOptionsMenu::ChangeTextureMedium()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.TextureQuality 1"));
}

void UOptionsMenu::ChangeTextureHigh()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.TextureQuality 2"));
}

void UOptionsMenu::ChangeTextureEpic()
{
	GetWorld()->Exec(GetWorld(), TEXT("sg.TextureQuality 3"));
}
