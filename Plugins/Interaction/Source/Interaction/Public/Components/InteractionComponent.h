// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterComponent.h"
#include "InteractionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INTERACTION_API UInteractionComponent : public UCharacterComponent
{
	GENERATED_BODY()

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bDebuggingMode = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float UseLength = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bStartTraceOnBeginPlay = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bDoesTraceTick = false;
	// How often the trace loops 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta=(EditCondition = "bDoesTraceTick"))
	float TraceTickRate = 0.1f;

	UFUNCTION(BlueprintCallable)
	void StartTraceForward();
	UFUNCTION(BlueprintCallable)
	void StopTraceForward();
	UFUNCTION(BlueprintCallable)
	void Use();

protected:
	virtual void BeginPlay() override;
	void TraceForward();
	
private:
	UPROPERTY()
	AActor* FocusedActor;
	FTimerHandle TraceForwardTimerHandle;
};
