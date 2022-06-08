// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ALSCustomCharacter.h"
#include "ALSCustomPlayerCharacter.generated.h"

UCLASS()
class TEMPLATEBUILDER_API AALSCustomPlayerCharacter : public AALSCustomCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AALSCustomPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	//Interaction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float UseLength;

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Menu")
	void PauseGame();
	virtual void PauseGame_Implementation();
	
	virtual void Death_Implementation() override;
	
protected:
	virtual void BeginPlay() override;


	////Interaction////
	void Use();
	UFUNCTION(Server, Reliable)
	void ServerUse();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TraceForward();
	void TraceForward_Implementation();
	UFUNCTION(Server, Reliable)
	void ServerTraceForward();

	virtual void AimPressedAction() override;
	virtual void AimReleasedAction() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartAiming();
	void StartAiming_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopAiming();
	void StopAiming_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimAssistLookRate = 1.0f;
private:
	//Interaction
	UPROPERTY(Replicated)
	AActor* FocusedActor;

	
	//functions
	void ThrowWeaponAction();

	void DebugCurrentWeapon();
	bool bDebuggingMode;


};
