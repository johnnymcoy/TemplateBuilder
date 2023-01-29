// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponentBase.h"
#include "CharacterHealthComponent.generated.h"

UENUM(BlueprintType)
enum class EInjuredState: uint8
{
	None,
	Head,
	LeftArm,
	RightArm,
	LeftLeg,
	RightLeg,
};

// On Shield Break sets off blueprint event
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShieldBreak);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInjuredBodyPart, EInjuredState, InjuredState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthAndShieldChanged,class UHealthComponentBase*, HealthComponent, float, Health, float, MaxHealth, float, ShieldHealth, float, MaxShieldHealth, const class UDamageType*, DamageType);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEALTH_API UCharacterHealthComponent : public UHealthComponentBase
{
	GENERATED_BODY()

public:

	UCharacterHealthComponent();
	// Use this if the Character has a shield
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthAndShieldChanged OnHealthAndShieldChanged;
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnInjuredBodyPart OnInjuredBodyPart;
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnShieldBreak OnShieldBreak;

	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE float GetShieldHealth() const { return ShieldHealth;}
	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE float GetMaxShieldHealth() const { return MaxShieldHealth;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Limbs", meta = (EditCondition = "!bUnrealSkeleton && !bUseCustomBones"))
	bool bSyntySkeleton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Limbs", meta = (EditCondition = "!bSyntySkeleton && !bUseCustomBones"))
	bool bUnrealSkeleton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Limbs", meta = (EditCondition = "!bSyntySkeleton && !bUnrealSkeleton"))
	bool bUseCustomBones;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> HeadBones;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> RightArmBones;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> LeftArmBones;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> RightLegBones;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> LeftLegBones; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> RightHandBones; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Limbs", meta = (EditCondition = "bUseCustomBones"))
	TArray<FName> LeftHandBones; 

	UFUNCTION()
	void LimbDamage(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone, TSubclassOf<class UDamageType> in_DamageType);

	UFUNCTION()
	void ShieldRegen();

protected:
	virtual void BeginPlay() override;

	virtual void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) override;
	virtual void TakePointDamage(AActor OnTakePointDamage, AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	// // Health Bar
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	// class UHealthBarWidgetComponent* HealthBar;


	UFUNCTION()
	void OnRep_Shield();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	const UDamageType* ShieldDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool bHasShield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (EditCondition = "bHasShield"))
	float MaxShieldHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (EditCondition = "bHasShield"))
	float ShieldTimeToRegen = 5.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	class UHealthWidget* HealthWidget;


private:
	UPROPERTY(ReplicatedUsing=OnRep_Shield, meta = (EditCondition = "bHasShield"))
	float ShieldHealth;

	float AmountToRecharge = 0.1f;
	
	//Limb counter?
	UPROPERTY(VisibleAnywhere, Category= "Health|Limbs")
	TArray<float> BodyIndex;
	
	void SetupSyntySkeleton();

	// virtual void HealthDamage(float Damage) override;
	
	void ShieldDamage(float Damage);

	void CheckShieldRegen();

	//Hit Counters on each limb
	float HeadHitCounter;
	float RightArmHitCounter;
	float LeftArmHitCounter;
	float RightLegHitCounter;
	float LeftLegHitCounter;

	FTimerHandle ShieldTimerHandle;

	void FaceHealthBarToPlayer();

	FTimerHandle HealthBarTimer;
	bool bHealthBarActive;



};
