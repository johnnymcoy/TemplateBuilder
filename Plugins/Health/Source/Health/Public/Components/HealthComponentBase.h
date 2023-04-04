// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterComponent.h"
#include "HealthComponentBase.generated.h"

// UENUM(BlueprintType)
// enum class EInjuredState: uint8
// {
// 	None,
// 	Head,
// 	LeftArm,
// 	RightArm,
// 	LeftLeg,
// 	RightLeg,
// };

// On Death sets off blueprint event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathEvent, AActor*, OwningActor);
// // On Shield Break sets off blueprint event
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShieldBreak);
//On Health Changed event, Sets off a Blueprint event when health is changed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged,class UHealthComponentBase*, HealthComponent, float, Health, float, MaxHealth,const class UDamageType*, DamageType);

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInjuredBodyPart, EInjuredState, InjuredState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEALTH_API UHealthComponentBase : public UCharacterComponent
{
	GENERATED_BODY()
	
public:	
	UHealthComponentBase();
	
	// Delegate fired when the health value has changed, Not for use on shielded Characters
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeathEvent OnDeath;
	// UPROPERTY(BlueprintAssignable, Category = "Health")
	// FOnInjuredBodyPart OnInjuredBodyPart;
	// UPROPERTY(BlueprintAssignable, Category = "Health")
	// FOnShieldBreak OnShieldBreak;


	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE float GetHealth() const { return Health; }
	UFUNCTION(BlueprintCallable, Category= "Health")
	FORCEINLINE float GetMaxHealth()const { return MaxHealth; };
	// UFUNCTION(BlueprintCallable, Category = "Health")
	// FORCEINLINE float GetShieldHealth() const { return ShieldHealth;}
	// UFUNCTION(BlueprintCallable, Category = "Health")
	// FORCEINLINE float GetMaxShieldHealth() const { return MaxShieldHealth;}

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "!bUnrealSkeleton && !bUseCustomBones"))
	// bool bSyntySkeleton;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "!bSyntySkeleton && !bUseCustomBones"))
	// bool bUnrealSkeleton;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "!bSyntySkeleton && !bUnrealSkeleton"))
	// bool bUseCustomBones;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	// TArray<FName> HeadBones;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	// TArray<FName> RightArmBones;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	// TArray<FName> LeftArmBones;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	// TArray<FName> RightLegBones;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	// TArray<FName> LeftLegBones; 
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	// TArray<FName> RightHandBones; 
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Limbs", meta = (EditCondition = "bUseCustomBones"))
	// TArray<FName> LeftHandBones; 


	// UFUNCTION()
	// void LimbDamage(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone, TSubclassOf<class UDamageType> in_DamageType);
	//
	// UFUNCTION()
	// void ShieldRegen();
	// float AmountToRecharge = 0.1f;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnRep_Health();
	// UFUNCTION()
	// void OnRep_Shield();
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// const UDamageType* ShieldDamageType;
	//
	//TODO: fix Damage types
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TSubclassOf<class UDamageType> ShieldDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	// bool bHasShield;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (EditCondition = "bHasShield"))
	// float MaxShieldHealth;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (EditCondition = "bHasShield"))
	// float ShieldTimeToRegen = 5.0f;
	UPROPERTY()
	AActor* MyOwner;

private:
	
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	float Health;
	// UPROPERTY(ReplicatedUsing=OnRep_Shield, meta = (EditCondition = "bHasShield"))
	// float ShieldHealth;

	bool bDeathOnce;
	virtual void Death();
	// UPROPERTY(Replicated)
	// bool bIsDead;
	//Limb counter?
	// UPROPERTY(VisibleAnywhere)
	// TArray<float> BodyIndex;
	
	// void SetupSyntySkeleton();
	//
	virtual void HealthDamage(float Damage);

	// void ShieldDamage(float Damage);
	//
	// void CheckShieldRegen();
	// // void HealthUpdateDelay();
	//
	// //Hit Counters on each limb
	// float HeadHitCounter;
	// float RightArmHitCounter;
	// float LeftArmHitCounter;
	// float RightLegHitCounter;
	// float LeftLegHitCounter;

	FTimerHandle HealthTimerHandle;
	// FTimerHandle ShieldTimerHandle;
	
};


