// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterComponent.h"
#include "DlgSystem/Public/DlgDialogueParticipant.h"
#include "Interfaces/DialogueInterface.h"
#include "DialogueComponent.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FDialogueDataValues
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<FName, int32> Integers;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<FName, int32> Floats;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<FName, FName> Names;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TSet<FName> TrueBools;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DIALOGUECOMPONENT_API UDialogueComponent : public UCharacterComponent, public IDlgDialogueParticipant, public IDialogueInterface
{
	GENERATED_BODY()

	UDialogueComponent();

	public:
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool GetIsInDialogue() const{return bIsInDialogue;}
	
	void InitialSetup();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Dialogue)
	bool StartDialogue(class UDlgDialogue* Dialogue, const TArray<UObject*>& Participants);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Dialogue)
	void SelectDialogueOption(int32 Option);
	//bool?
protected:
	virtual void BeginPlay() override;


	// Interface Values
	// Name of this participant
	// Used for GetParticipantName
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	FName DialogueParticipantName = FName("ExampleParticipantName");

	// UI name of this participant, what is displayed inside the UI
	// Used for GetParticipantDisplayName
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	FText DialogueParticipantDisplayName = NSLOCTEXT("ExampleNamespace", "ExampleCharacterName", "ExampleParticipantName");

	// Used for GetParticipantIcon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	UTexture2D* DialogueParticipantIcon;

	// Context used to control the Dialogue follow
	UPROPERTY(BlueprintReadWrite, Category = Dialogue)
	UDlgContext* DialogueContext = nullptr;

	// // Struct used to get/set the Dialogue Values
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	FDialogueDataValues DialogueDataStruct;

	// Interface Functions
	FName GetParticipantName_Implementation() const override { return DialogueParticipantName; }
	FText GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const override {return DialogueParticipantDisplayName;}
	UTexture2D* GetParticipantIcon_Implementation(FName ActiveSpeaker, FName ActiveSpeakerState) const override {return DialogueParticipantIcon;}
	ETextGender GetParticipantGender_Implementation() const override { return ETextGender::Neuter; }

	bool ModifyIntValue_Implementation(FName ValueName, bool bDelta, int32 Value) override;
	bool ModifyFloatValue_Implementation(FName ValueName, bool bDelta, float Value) override;
	bool ModifyBoolValue_Implementation(FName ValueName, bool bValue) override;
	bool ModifyNameValue_Implementation(FName ValueName, FName NameValue) override;

	float GetFloatValue_Implementation(FName ValueName) const override;
	int32 GetIntValue_Implementation(FName ValueName) const override;
	bool GetBoolValue_Implementation(FName ValueName) const override;
	FName GetNameValue_Implementation(FName ValueName) const override;

	bool OnDialogueEvent_Implementation(UDlgContext* Context, FName EventName) override;
	bool CheckCondition_Implementation(const UDlgContext* Context, FName ConditionName) const override;

	
	UPROPERTY(BlueprintReadWrite, Category = Dialogue)
	class UDlgContext* ActiveContext;
	
private:
	
	

	
	//Widget
	UPROPERTY()
	class UDialogueWidget* DialogueWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> DialogueWidgetClass;

	bool bIsInDialogue;

};
