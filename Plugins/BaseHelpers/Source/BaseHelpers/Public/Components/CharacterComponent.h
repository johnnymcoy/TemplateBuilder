// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/ActorComponent.h"
#include "CharacterComponent.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASEHELPERS_API UCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterComponent();
	UFUNCTION(BlueprintCallable)
	void SetupComponent(USkeletalMeshComponent* SkeletalMesh, UAnimInstance* AnimationInstance, AController* Controller, const bool bNPC, const bool bDead);
	void SetOwnerMesh(USkeletalMeshComponent* SkeletalMesh){OwnerMesh = SkeletalMesh;}
	void SetAnimInstance(UAnimInstance* AnimationInstance){MainAnimInstance = AnimationInstance;};
	void SetController(AController* Controller);
	void SetIsNPC(const bool bNPC){bIsNPC = bNPC;};
	void SetIsDead(const bool bDead){bIsDead = bDead;};

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "Character")
	AController* GetOwnerController();
	UFUNCTION(BlueprintCallable, Category = "Character")
	APlayerController* GetOwnerPlayerController();
	UFUNCTION(BlueprintCallable, Category = "Character")
	USkeletalMeshComponent* GetOwnerMesh();
	UFUNCTION(BlueprintCallable, Category = "Character")
	UAnimInstance* GetOwnerAnimInstance();
	UFUNCTION(BlueprintCallable, Category = "Character")
	AAIController* GetOwnerAIController();

	UFUNCTION(BlueprintCallable, Category = "Character")
	bool GetIsNPC() const {return bIsNPC;};
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool GetIsDead() const {return bIsDead;};

	void SetInputModeGameAndUI(bool bGameAndUI, class UWidget* InWidgetToFocus, bool bShowMouse);

	bool bIsComponentSetup;
	
	void LogMissingPointer(FString PointerName) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debugging")
	bool bDebuggingMode;

	//- Play Animation //
	UFUNCTION(Server, Unreliable)
	void Server_PlayMontageAnimation(UAnimMontage* MontageToPlay, float InPlayRate = 1.0f, EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::Duration, float InTimeToStartMontageAt = 0.0f, bool bStopAllMontages = true);
	UFUNCTION(Server, Unreliable)
	void Server_StopMontageAnimation(float InBlendOutTime, const UAnimMontage* Montage);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float LastAnimationDuration = 0.0f;
private:

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayMontageAnimation(UAnimMontage* MontageToPlay, float InPlayRate = 1.0f, EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::Duration, float InTimeToStartMontageAt = 0.0f, bool bStopAllMontages = true);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_StopMontageAnimation(float InBlendOutTime, const UAnimMontage* Montage);
	
	bool CheckComponentIsSetup(FString ComponentName);
	bool bIsNPC;
	bool bIsDead;
	UPROPERTY()
	AController* OwnerController;
	UPROPERTY()
	APlayerController* OwnerPlayerController;
	UPROPERTY()
	AAIController* OwnerAIController;
	UPROPERTY()
	USkeletalMeshComponent* OwnerMesh;
	UPROPERTY()
	UAnimInstance* MainAnimInstance;

};


