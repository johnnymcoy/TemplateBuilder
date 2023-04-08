// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CustomCharacterBase.h"
#include "CustomPlayerCharacter.generated.h"

UCLASS()
class CUSTOMALS_API ACustomPlayerCharacter : public ACustomCharacterBase
{
	GENERATED_BODY()
public:
	ACustomPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//- Components // 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Shooting")
	class UPlayerCharacterShootingComponent* PlayerShootingComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Companion")
	class UCompanionMasterComponent* CompanionMasterComponentComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Proximity Chat")
	class UPlayerVoiceComponent* ProximityVoiceComponent;

	virtual void BeginPlay() override;

	
	// Parent Functions
	virtual void AimPressedAction() override;
	virtual void AimReleasedAction() override;
	virtual void FirePressedAction() override;
	

	//- Character Weapons Interface
	virtual int32 PickupGunEvent(const FWeaponData_T In_WeaponData) override;
	//- Shooting Component Bind //
	virtual void WeaponEquipped(TArray<FWeaponData_T> Weapons, int32 CurrentWeaponIndex) override;


	// Interaction
	void UseAction();

	//-		Overridden from CustomCharacter
	virtual void OnDeath(AActor* OwningActor) override;

	virtual void SetupComponents() override;

	//-		Voice Chat	//
	// UFUNCTION(Server, Reliable, WithValidation)
	// void SendVoiceChat(const TArray<uint8>& VoiceData, int32 SampleRate);
	// UFUNCTION(BlueprintCallable, Category="Proximity Chat")
	// void OnVoiceChatReceived(APlayerController* SenderPlayerController, const TArray<uint8>& VoiceData, int32 SampleRate);

	
private:
	
	void UpdateCompanionContext();
	FTimerHandle CompanionContextTimerHandle;
	


};
