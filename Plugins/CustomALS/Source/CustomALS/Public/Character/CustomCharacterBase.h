// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ALSCharacter.h"
#include "GameFramework/Character.h"
#include "Interfaces/CustomCharacter.h"
#include "CustomCharacterBase.generated.h"

UCLASS()
class CUSTOMALS_API ACustomCharacterBase : public AALSCharacter, public ICustomCharacter
{
	GENERATED_BODY()

public:
	ACustomCharacterBase(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Helper Functions
	UFUNCTION(BlueprintPure)
	bool IsDead() const;
	UFUNCTION(BlueprintPure)
	bool IsCrouching() const;

	// Character Interface
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	virtual void GetDialogueComponent(UDialogueComponent* &Out_DialogueComponent) const override;


protected:
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Health")
	class UCharacterHealthComponent* CharacterHealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	class UDialogueComponent* DialogueComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Shooting")
	class UCharacterShootingComponent* ShootingComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Interaction")
	class UInteractionComponent* InteractionComponent;

	
	// Health Component Functions 
	UFUNCTION()
	virtual void OnHealthChanged(class UHealthComponentBase* HealthComponent, float Health, float MaxHealth, const class UDamageType* DamageType);
	UFUNCTION()
	virtual void OnHealthAndShieldChanged(class UHealthComponentBase* HealthComponent, float Health, float MaxHealth, float ShieldHealth, float MaxShieldHealth, const class UDamageType* DamageType);
	UFUNCTION()
	virtual void OnInjuredBodyPart(FName InjuredBodyPart);
	UFUNCTION()
	virtual void OnShieldBreak();
	UFUNCTION()
	virtual void OnDeath(AActor* OwningActor);


	
	
	bool bIsNPC;
	bool bIsDead;

	
private:

	///////////////// Input Functions ////////////////////
	// virtual void AimPressedAction() override;
	// virtual void AimReleasedAction() override;

	void FirePressedAction();
	// void FireReleasedAction();
	// void ReloadPressedAction();
	//
	// void SwapWeaponPressedAction();
	// void SwapWeaponReleasedAction();
	//
	// void SwitchAutoModePressedAction();
	// void ThrowWeaponPressedAction();
	
	/////////////////  //////////////////// ////////////////////


	

	
};
