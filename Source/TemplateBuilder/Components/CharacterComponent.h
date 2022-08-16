// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/ActorComponent.h"
#include "CharacterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATEBUILDER_API UCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterComponent();
	void SetOwnerMesh(USkeletalMeshComponent* SkeletalMesh){OwnerMesh = SkeletalMesh;};
	void SetAnimInstance(UAnimInstance* AnimationInstance){MainAnimInstance = AnimationInstance;};
	void SetController(AController* Controller);
	void SetIsNPC(bool in_bIsNPC){bIsNPC = in_bIsNPC;};
	void SetIsDead(bool bDead){bIsDead = bDead;};
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "Character")
	AController* GetOwnerController() const {return OwnerController;};
	UFUNCTION(BlueprintCallable, Category = "Character")
	APlayerController* GetOwnerPlayerController() const {return OwnerPlayerController;};
	UFUNCTION(BlueprintCallable, Category = "Character")
	AAIController* GetOwnerAIController() const {return OwnerAIController;};
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool GetIsNPC() const {return bIsNPC;};
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool GetIsDead() const {return bIsDead;};

private:
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
