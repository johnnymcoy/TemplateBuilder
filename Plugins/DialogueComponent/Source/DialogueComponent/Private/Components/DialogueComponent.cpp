// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DialogueComponent.h"
#include "Blueprint/UserWidget.h"
#include "DlgSystem/Public/DlgContext.h"
#include "DlgSystem/Public/DlgDialogue.h"
#include "DlgSystem/Public/DlgManager.h"
#include "Widgets/DialogueWidget.h"
// #include "UObject/ConstructorHelpers.h"

// T:/Unreal/Unreal Projects/TemplateBuilder/Plugins/DialogueComponent/Content/Widgets/Dialogue_WBP.uasset
// WidgetBlueprint'/DialogueComponent/Widgets/Dialogue_WBP.Dialogue_WBP'

UDialogueComponent::UDialogueComponent()
{
	// Todo Fix up Reference to Dialogue WBP so it doesn't have to be manually added 
	// ConstructorHelpers::FClassFinder<UUserWidget> DialogueWidget_WBP(TEXT("/DialogueComponent/Widgets/Dialogue_WBP"));
	// if (!ensure(DialogueWidget_WBP.Class != nullptr)) return;
	// DialogueWidgetClass = DialogueWidget_WBP.Class;
}

void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDialogueComponent::InitialSetup()
{
	if(GetIsNPC()){return;}
	//Check values are valid
	
	if(DialogueWidgetClass == nullptr){UE_LOG(LogTemp, Error, TEXT("Add DialogueWidgetClass in Dialogue Component")); return;}
	if(GetOwnerPlayerController() == nullptr){UE_LOG(LogTemp, Error, TEXT("Player Controller Failed in Dialogue Component")) return;}

	DialogueWidget = CreateWidget<UDialogueWidget>(GetOwnerPlayerController(), DialogueWidgetClass);
	if(DialogueWidget == nullptr){UE_LOG(LogTemp, Error, TEXT("Dialogue Widget FAILED to create"))return;}
	DialogueWidget->AddToViewport();
	DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
	if(DialogueWidget != nullptr){UE_LOG(LogTemp, Error, TEXT("Dialogue Widget SUCCESS"));}

}

bool UDialogueComponent::StartDialogue_Implementation(UDlgDialogue* Dialogue, const TArray<UObject*>& Participants)
{
	UE_LOG(LogTemp, Warning, TEXT("Start Dialogue from Dialogue Component"));
	ActiveContext = UDlgManager::StartDialogue(Dialogue, Participants);
	DialogueWidget->Refresh(ActiveContext);
	SetInputModeGameAndUI(true, nullptr, true);
	bIsInDialogue = true;
	return ActiveContext != nullptr;
}

void UDialogueComponent::SelectDialogueOption_Implementation(int32 Option)
{
	if(ActiveContext == nullptr){UE_LOG(LogTemp, Error, TEXT("Active Dialogue Context is Invalid in Dialogue Component"))return;}
	bool bIsStillActive = ActiveContext->ChooseOption(Option);
	if(bIsStillActive)
	{
		DialogueWidget->Refresh(ActiveContext);
	}
	else
	{
		bIsInDialogue = false;
		DialogueWidget->Refresh(nullptr);
		SetInputModeGameAndUI(false, nullptr, false);		
	}
}


bool UDialogueComponent::ModifyIntValue_Implementation(FName ValueName, bool bDelta, int32 Value)
{
	if (!DialogueDataStruct.Integers.Contains(ValueName))
		DialogueDataStruct.Integers.Add(ValueName, 0);

	if (bDelta)
		DialogueDataStruct.Integers[ValueName] += Value;
	else
		DialogueDataStruct.Integers[ValueName] = Value;

	return true;
}

bool UDialogueComponent::ModifyFloatValue_Implementation(FName ValueName, bool bDelta, float Value)
{
	if (!DialogueDataStruct.Floats.Contains(ValueName))
		DialogueDataStruct.Floats.Add(ValueName, 0.0f);

	if (bDelta)
		DialogueDataStruct.Floats[ValueName] += Value;
	else
		DialogueDataStruct.Floats[ValueName] = Value;

	return true;
}

bool UDialogueComponent::ModifyBoolValue_Implementation(FName ValueName, bool bValue)
{
	if (bValue)
		DialogueDataStruct.TrueBools.Add(ValueName);
	else
		DialogueDataStruct.TrueBools.Remove(ValueName);

	return true;
}

bool UDialogueComponent::ModifyNameValue_Implementation(FName ValueName, FName NameValue)
{
	if (DialogueDataStruct.Names.Contains(ValueName))
		DialogueDataStruct.Names[ValueName] = NameValue;
	else
		DialogueDataStruct.Names.Add(ValueName, NameValue);

	return true;
}

float UDialogueComponent::GetFloatValue_Implementation(FName ValueName) const
{
	return DialogueDataStruct.Floats.Contains(ValueName) ? DialogueDataStruct.Floats[ValueName] : 0.0f;
}

int32 UDialogueComponent::GetIntValue_Implementation(FName ValueName) const
{
	return DialogueDataStruct.Integers.Contains(ValueName) ? DialogueDataStruct.Integers[ValueName] : 0;
}

bool UDialogueComponent::GetBoolValue_Implementation(FName ValueName) const
{
	return DialogueDataStruct.TrueBools.Contains(ValueName);
}

FName UDialogueComponent::GetNameValue_Implementation(FName ValueName) const
{
	return DialogueDataStruct.Names.Contains(ValueName) ? DialogueDataStruct.Names[ValueName] : NAME_None;
}

bool UDialogueComponent::OnDialogueEvent_Implementation(UDlgContext* Context, FName EventName)
{
	// Used for Dialogue based events
	return false;
}

bool UDialogueComponent::CheckCondition_Implementation(const UDlgContext* Context, FName ConditionName) const
{
	// Used for Dialogue based conditions
	return false;
}
