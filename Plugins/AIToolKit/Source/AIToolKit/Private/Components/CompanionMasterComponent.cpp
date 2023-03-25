// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CompanionMasterComponent.h"
#include "Interfaces/CompanionInterface.h"
#include "Widgets/CompanionWidget.h"

UCompanionMasterComponent::UCompanionMasterComponent()
{
	//-		Get Default BP Classes for Widgets		//
	ConstructorHelpers::FClassFinder<UCompanionWidget> const CompanionWidgetBPClass(TEXT("/AIToolKit/Widgets/CompanionWidget_WBP"));
	if (!ensure(CompanionWidgetBPClass.Class != nullptr)) return;
	CompanionWidgetClass = CompanionWidgetBPClass.Class;
}

void UCompanionMasterComponent::SetupCompanionWidget()
{
	if(GetOwnerController() == nullptr){LogMissingPointer("Owner Controller");return;}
	if(GetOwnerController()->IsLocalController())
	{
		if(CompanionWidgetClass == nullptr){LogMissingPointer("Companion Widget Class");return;}
		if(GetOwnerPlayerController() == nullptr){LogMissingPointer("Owner Player Controller"); return;}
		CompanionWidget = CreateWidget<UCompanionWidget>(GetOwnerPlayerController(), CompanionWidgetClass);
		if(CompanionWidget == nullptr){LogMissingPointer("Companion Widget Creation Failed"); return;}
		CompanionWidget->AddToViewport();
	}
}

void UCompanionMasterComponent::SetCompanionToCommand(AActor* NewCompanion)
{
	//- Check if it is a Actor we can Command // 
	TArray<UActorComponent*> Components = NewCompanion->GetComponentsByInterface(UCompanionInterface::StaticClass());
	if(Components.IsValidIndex(0))
	{
		ICompanionInterface* Companion = Cast<ICompanionInterface>(Components[0]);
		if(Companion != nullptr)
		{
			CompanionActor = NewCompanion;
			Companion->SetMaster(GetOwner());
			bCommandingCompanion = true;
			CompanionWidget->SetCompanionName(FName(Companion->GetDisplayName()));
			// CompanionWidget->SetCompanionName(CompanionActor->GetName());
		}
	}
}

void UCompanionMasterComponent::CommandCompanion(const FHitResult HitResult)
{
	if(CompanionActor != nullptr)
	{
		const TArray<UActorComponent*> Components = CompanionActor->GetComponentsByInterface(UCompanionInterface::StaticClass());
		if(Components.IsValidIndex(0))
		{
			ICompanionInterface* Companion = Cast<ICompanionInterface>(Components[0]);
			if(HitResult.Actor == CompanionActor && Companion != nullptr)
			{
				Companion->Follow();
			}
			else if(Companion != nullptr)
			{
				Companion->Wait(HitResult.Location);
			}
		}

		// if(Enemy)
		{
			//attack
		}
		// if()
		bCommandingCompanion = false;
	}
	// TArray<UActorComponent*> Components = CompanionToCommand->GetComponentsByInterface(UCompanionInterface::StaticClass());
	// if(Components.IsValidIndex(0))
	// {
	// 	ICompanionInterface* Companion = Cast<ICompanionInterface>(Components[0]);
	// 	if(Companion != nullptr)
	// 	{
	// 		Companion;
	// 		//UE_LOG(LogTemp, Warning, TEXT(" Companion Interface- Master Component"));
	// 	}
	// }
	// ICompanionInterface* Companion = Cast<ICompanionInterface>(CompanionToCommand);
	// if(Companion != nullptr)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT(" Companion Interface- Master Component"));
	//
	// }
	// if(bCommandingCompanion)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Command Companion- Master Component"));
	// }
}
