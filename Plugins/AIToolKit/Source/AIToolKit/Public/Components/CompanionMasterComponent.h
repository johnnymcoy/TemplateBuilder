// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterComponent.h"
#include "CompanionMasterComponent.generated.h"

UCLASS()
class AITOOLKIT_API UCompanionMasterComponent : public UCharacterComponent
{
	GENERATED_BODY()

public:

	UCompanionMasterComponent();

	UFUNCTION(BlueprintCallable, Category="Companion|Master")
	void SetupCompanionWidget();
	
	UFUNCTION(BlueprintCallable, Category="Companion")
	void SetCompanionToCommand(AActor* NewCompanion);
	
	UFUNCTION(BlueprintCallable, Category="Companion")
	void CommandCompanion(const FHitResult HitResult);

	UFUNCTION(BlueprintCallable, Category="Companion")
	bool GetIsCommandingCompanion(){return bCommandingCompanion;}

	UFUNCTION(BlueprintCallable, Category="Companion")
	void UpdateInteractionTarget(FHitResult InteractionHits);
	
protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Companions")
	bool bCommandingCompanion;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Companions")
	AActor* CompanionActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Companions")
	class UCompanionWidget* CompanionWidget;
private:
	//- Widgets // 
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCompanionWidget> CompanionWidgetClass;


};
