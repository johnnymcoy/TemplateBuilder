// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterComponent.h"
#include "Interfaces/CompanionInterface.h"
#include "Data/CompanionDataTypes.h"
#include "CompanionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AITOOLKIT_API UCompanionComponent : public UCharacterComponent, public ICompanionInterface
{
	GENERATED_BODY()

public:	
	UCompanionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void SetMaster(AActor* Master) override;
	UFUNCTION(BlueprintCallable, Category="Companions")
	void SetupBlackboard();

	//-			Interface Functions			-//
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void Follow() override;
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void Fetch(UPrimitiveComponent* ComponentToGrab) override;
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void StartCombat(AActor* TargetEnemy) override;
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void Wait(FVector TargetLocation) override;
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void GrabObject() override;
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual FString GetDisplayName() override;

	//-			AI Interface					//
	UFUNCTION(BlueprintCallable, Category="Companions")
	virtual void Attack() override;

	FString DisplayName;

protected:
	virtual void BeginPlay() override;

	
	UFUNCTION(BlueprintCallable, Category="Companions")
	void UpdateCompanionState(ECompanionState NewState);

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Companions")
	UAnimMontage* AttackAnimMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Companions")
	UAnimMontage* GrabAnimMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Companions")
	ECompanionState CompanionState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Companions")
	bool bHasMaster;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Companions")
	bool bUseGrabAnimation;

	//- todo		//
	//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Companions")
	//	float FollowDistance;
	//- Used to tag object available to pickup by companion - change to interface //
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Companions")
	// FName ComponentPickupTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Companions")
	FName GrabObjectSocketName;
	
private:
	
	UPROPERTY(VisibleAnywhere, Category="Companions")
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(VisibleAnywhere, Category="Companions")
	UPrimitiveComponent* ObjectToGrab;
	UPROPERTY(VisibleAnywhere, Category="Companions")
	AActor* TargetActor;
	
	UPROPERTY(VisibleAnywhere, Category="Companions")
	AActor* MasterActor;


	void OnGrabObject();
	FTimerHandle GrabTimerHandle;

	
};
