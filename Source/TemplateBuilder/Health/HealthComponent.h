// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

//On Health Changed event, Sets off a Blueprint event when health is changed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, Health, float, DefaultHealth, float, Shield, float, DefaultShield, const class UDamageType*, DamageType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATEBUILDER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE float GetHealth() const { return Health; }
	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE float GetShieldHealth() const { return ShieldHealth;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DefaultHealth;
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly)
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool bHasShield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (EditCondition = "bHasShield"))
	float DefaultShieldHealth;
	UPROPERTY(ReplicatedUsing=OnRep_Shield, BlueprintReadOnly, Category = "Health", meta = (EditCondition = "bHasShield"))
	float ShieldHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "!bUnrealSkeleton && !bUseCustomBones"))
	bool bSyntySkeleton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "!bSyntySkeleton && !bUseCustomBones"))
	bool bUnrealSkeleton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "!bSyntySkeleton && !bUnrealSkeleton"))
	bool bUseCustomBones;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> HeadBones;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> RightArmBones;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> LeftArmBones;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> RightLegBones;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> LeftLegBones; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> RightHandBones; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> LeftHandBones; 

	UFUNCTION()
	void LimbDamage(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone, TSubclassOf<class UDamageType> in_DamageType);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnRep_Health();
	UFUNCTION()
	void OnRep_Shield();


private:	
	//Limb counter?
	UPROPERTY(VisibleAnywhere)
	TArray<float> BodyIndex;

	void SetupSyntySkeleton();


	//Hit Counters on each limb
	float HeadHitCounter;
	float RightArmHitCounter;
	float LeftArmHitCounter;
	float RightLegHitCounter;
	float LeftLegHitCounter;

		
};
