// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "TemplateBuilder/DamageTypes/ShieldRegen.h"

static int32 DebugHealthStats = 0;
FAutoConsoleVariableRef CVARDebugHealthStats(TEXT("CustomDebugs.DebugHealthStats"), DebugHealthStats, TEXT("Debugs for HealthComponents"), ECVF_Cheat);

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	SetIsReplicatedByDefault(true);
	DefaultHealth = 100;
	Health = DefaultHealth;
	DefaultShieldHealth = 100;
	ShieldHealth = DefaultShieldHealth;
	BodyIndex.Init(0,7);
	//todo newObjkecT? 
	ShieldDamageType = NewObject<UShieldRegen>();
	if(bSyntySkeleton){SetupSyntySkeleton();}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if(MyOwner != nullptr)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
		}
	}
}

void UHealthComponent::OnRep_Health() 
{
	OnHealthChanged.Broadcast(this, Health, DefaultHealth, ShieldHealth, DefaultShieldHealth, nullptr);
}

void UHealthComponent::OnRep_Shield() 
{
	OnHealthChanged.Broadcast(this, Health, DefaultHealth, ShieldHealth, DefaultShieldHealth, nullptr);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) 
{
	if(bHasShield)
	{
		if(ShieldHealth > 0 && Damage > 0)
		{
			ShieldDamage(Damage);
		}
		else
		{
			HealthDamage(Damage);
		}
	}
	else
	{
		HealthDamage(Damage);
	}
	if(DebugHealthStats){UE_LOG(LogTemp,Warning, TEXT("Health of %s is: %s"),*FString::SanitizeFloat(Health), *GetOwner()->GetName());}
	OnHealthChanged.Broadcast(this, Health, DefaultHealth, ShieldHealth, DefaultShieldHealth, DamageType);
}

void UHealthComponent::ShieldDamage(float Damage)
{
	ShieldHealth = FMath::Clamp(ShieldHealth - Damage, 0.0f, DefaultShieldHealth);
}

void UHealthComponent::HealthDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
}


void UHealthComponent::SetupSyntySkeleton() 
{
	HeadBones.Add(FName("head"));
	HeadBones.Add(FName("eyes"));
	HeadBones.Add(FName("eyebrows"));
	RightArmBones.Add(FName("clavicle_r"));
	RightArmBones.Add(FName("UpperArm_R"));
	RightArmBones.Add(FName("lowerarm_r"));
	LeftArmBones.Add(FName("clavicle_l"));
	LeftArmBones.Add(FName("UpperArm_L"));
	LeftArmBones.Add(FName("lowerarm_l"));
	RightLegBones.Add(FName("Thigh_R"));
	RightLegBones.Add(FName("calf_r"));
	RightLegBones.Add(FName("Foot_R"));
	RightLegBones.Add(FName("ball_r"));
	RightLegBones.Add(FName("toes_r"));
	LeftLegBones.Add(FName("Thigh_L"));
	LeftLegBones.Add(FName("calf_l"));
	LeftLegBones.Add(FName("Foot_L"));
	LeftLegBones.Add(FName("ball_l"));
	LeftLegBones.Add(FName("toes_l"));
	LeftHandBones.Add(FName("Hand_L"));
	LeftHandBones.Add(FName("thumb_01_l"));
	LeftHandBones.Add(FName("thumb_02_l"));
	LeftHandBones.Add(FName("thumb_03_l"));
	LeftHandBones.Add(FName("indexFinger_01_l"));
	LeftHandBones.Add(FName("indexFinger_02_l"));
	LeftHandBones.Add(FName("indexFinger_03_l"));
	LeftHandBones.Add(FName("indexFinger_04_l"));
	LeftHandBones.Add(FName("finger_01_l"));
	LeftHandBones.Add(FName("finger_02_l"));
	RightHandBones.Add(FName("hand_r"));
	RightHandBones.Add(FName("thumb_01_r"));
	RightHandBones.Add(FName("thumb_02_r"));
	RightHandBones.Add(FName("thumb_03_r"));
	RightHandBones.Add(FName("indexFinger_01_r"));
	RightHandBones.Add(FName("indexFinger_02_r"));
	RightHandBones.Add(FName("indexFinger_03_r"));
	RightHandBones.Add(FName("indexFinger_04_r"));
	RightHandBones.Add(FName("finger_01_r"));
	RightHandBones.Add(FName("finger_02_r"));
}


void UHealthComponent::LimbDamage(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone, TSubclassOf<class UDamageType> in_DamageType) 
{
	// UE_LOG(LogTemp, Warning, TEXT("HitBOne: %s"), *in_HitBone.ToString());
	for(int i = 0; i < HeadBones.Num(); i++)
	{
		if(in_HitBone == HeadBones[i])
		{
			BodyIndex[0] = BodyIndex[0] + (in_Damage * in_HeadMultiplier);
			// UE_LOG(LogTemp, Warning, TEXT("HeadHitCounter: %f"), BodyIndex[0]);
		}
	}
	for(int i = 0; i < RightArmBones.Num(); i++)
	{
		if(in_HitBone == RightArmBones[i])
		{
			BodyIndex[1] = BodyIndex[1] + (in_Damage * in_HeadMultiplier);
			// UE_LOG(LogTemp, Warning, TEXT("RightArmBones: %f"), BodyIndex[1]);
		}
	}
	for(int i = 0; i < LeftArmBones.Num(); i++)
	{
		if(in_HitBone == LeftArmBones[i])
		{
			BodyIndex[2] = BodyIndex[2] + (in_Damage * in_HeadMultiplier);
			// UE_LOG(LogTemp, Warning, TEXT("LeftArmBones: %f"), BodyIndex[2]);
		}
	}
	for(int i = 0; i < RightLegBones.Num(); i++)
	{
		if(in_HitBone == RightLegBones[i])
		{
			BodyIndex[3] = BodyIndex[3] + (in_Damage * in_HeadMultiplier);
		}
	}
	for(int i = 0; i < LeftLegBones.Num(); i++)
	{
		if(in_HitBone == LeftLegBones[i])
		{
			BodyIndex[4] = BodyIndex[4] + (in_Damage * in_HeadMultiplier);
		}
	}
	for(int i = 0; i < RightHandBones.Num(); i++)
	{
		if(in_HitBone == RightHandBones[i])
		{
			BodyIndex[5] = BodyIndex[5] + (in_Damage * in_HeadMultiplier);
			// UE_LOG(LogTemp, Warning, TEXT("RightHandBones: %f"), BodyIndex[5]);

		}
	}
	for(int i = 0; i < LeftHandBones.Num(); i++)
	{
		if(in_HitBone == LeftHandBones[i])
		{
			BodyIndex[6] = BodyIndex[6] + (in_Damage * in_HeadMultiplier);
			// UE_LOG(LogTemp, Warning, TEXT("LeftHandBones: %f"), BodyIndex[6]);
		}
	}
}

void UHealthComponent::ShieldRegen()
{
	ShieldHealth = FMath::Clamp(ShieldHealth + AmountToRecharge, 0.0f, DefaultShieldHealth);
	OnHealthChanged.Broadcast(this, Health, DefaultHealth, ShieldHealth, DefaultShieldHealth, ShieldDamageType);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, ShieldHealth);
}
