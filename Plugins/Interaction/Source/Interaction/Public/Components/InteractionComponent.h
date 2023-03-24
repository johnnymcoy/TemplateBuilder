// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterComponent.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTraceEvent, FHitResult, HitResult);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INTERACTION_API UInteractionComponent : public UCharacterComponent
{
	GENERATED_BODY()

	
public:
	UInteractionComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnTraceEvent OnTraceEventHit;
	
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
	AActor* Use();
	UFUNCTION(BlueprintCallable)
	FHitResult GetHitResult();

	UFUNCTION(BlueprintCallable)
	FVector GetTraceEndLocation();

protected:
	virtual void BeginPlay() override;
	void TraceForward();
	
private:
	UPROPERTY()
	AActor* FocusedActor;
	UPROPERTY()
	FVector CurrentTraceEnd = FVector::ZeroVector;
	UPROPERTY()
	FHitResult CurrentHitResult;
	
	FTimerHandle TraceForwardTimerHandle;

	void FocusOnActor(bool bStartFocus, AActor* ActorToFocus);
};
