// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ALSCustomCharacter.h"
#include "ALSCustomPlayerCharacter.generated.h"

UCLASS()
class TEMPLATEBUILDER_API AALSCustomPlayerCharacter : public AALSCustomCharacter
{
	GENERATED_BODY()

protected: 
	////Interaction////
	void Use();
	UFUNCTION(Server, Reliable)
	void ServerUse();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TraceForward();
	void TraceForward_Implementation();
	UFUNCTION(Server, Reliable)
	void ServerTraceForward();

	virtual void AimPressedAction();
	virtual void AimReleasedAction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartAiming();
	void StartAiming_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopAiming();
	void StopAiming_Implementation();

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
	
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ResumeGame();



private:
	//Interaction
	UPROPERTY(Replicated)
	AActor* FocusedActor;

	
	//functions
	void ThrowWeaponAction();

	void DebugCurrentWeapon();
	bool bDebuggingMode;


};
