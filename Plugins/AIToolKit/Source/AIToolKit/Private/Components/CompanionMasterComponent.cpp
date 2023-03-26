// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CompanionMasterComponent.h"

#include "Interfaces/AICharacter.h"
#include "Interfaces/CompanionInterface.h"
#include "Interfaces/InteractionInterface.h"
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
			CompanionWidget->SetIsCommanding(bCommandingCompanion);
			CompanionWidget->SetCompanionName(FName(Companion->GetDisplayName()));
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
			IAICharacter* AICharacter = Cast<IAICharacter>(HitResult.GetActor());
			IInteractionInterface* InteractableActor = Cast<IInteractionInterface>(HitResult.GetActor());
			//-		if just aiming at companion while commanding // 
			if(HitResult.Actor == CompanionActor && Companion != nullptr)
			{
				Companion->Follow();
				if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("Follow Command"));}
			}
			//-		if we're aimed at an AI character	//
			else if(AICharacter != nullptr && Companion != nullptr)
			{
				Companion->StartCombat(HitResult.GetActor());
				if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("Attack Command"));}
			}
			//-		if aimed at an interactable	object	//
			else if(InteractableActor && Companion != nullptr)
			{
				Companion->Fetch(HitResult.GetComponent());
				if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("Grab Object Command"));}
			}
			//-		Aimed at just a location	//
			else if(Companion != nullptr)
			{
				Companion->Wait(HitResult.Location);
				if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("Wait at Location Command"));}
			}
		}
		// if(Enemy)
		{
			//attack
		}
		// if()
		bCommandingCompanion = false;
		CompanionWidget->SetIsCommanding(bCommandingCompanion);
	}
}

void UCompanionMasterComponent::UpdateInteractionTarget(FHitResult InteractionHits)
{
	//- Only do if we're commanding //
	if(!bCommandingCompanion){return;}
	//- Make sure our Companion Actor is Valid	//
	if(CompanionActor == nullptr){LogMissingPointer("Companion Actor");return;}
	const TArray<UActorComponent*> Components = CompanionActor->GetComponentsByInterface(UCompanionInterface::StaticClass());
	//- Get the Companion Component from the Actor	//
	if(Components.IsValidIndex(0))
	{
		ICompanionInterface* Companion = Cast<ICompanionInterface>(Components[0]);
		IAICharacter* AICharacter = Cast<IAICharacter>(InteractionHits.GetActor());
		IInteractionInterface* InteractableActor = Cast<IInteractionInterface>(InteractionHits.GetActor());
		//-		if just aiming at companion while commanding // 
		if(InteractionHits.Actor == CompanionActor)
		{
			CompanionWidget->SetInteractionPrompt("Follow");
			if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("Follow Prompt"));}
		}
		//-		if we're aimed at an AI character	//
		else if(AICharacter != nullptr)
		{
			const FString CharacterName = AICharacter->GetCharacterDisplayName();
			CompanionWidget->SetInteractionPrompt(FString("Attack " + CharacterName));
			// Companion->StartCombat(InteractionHits.GetActor());
			if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("Attack Prompt"));}
		}
		//-		if aimed at an interactable	object	//
		else if(InteractableActor)
		{
			const FString ObjectName = InteractableActor->GetObjectDisplayName();
			CompanionWidget->SetInteractionPrompt(FString("Grab " + ObjectName));
			if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("Grab Object Prompt"));}
		}
		else if(Companion != nullptr)
		{
			CompanionWidget->SetInteractionPrompt("Wait Here");
			if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("Wait at Location Prompt"));}
		}
	}
}
