// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterHealthComponent.h"
#include "Components/HealthBarWidgetComponent.h"
#include "Widgets/HealthWidget.h"
#include "Net/UnrealNetwork.h"

UCharacterHealthComponent::UCharacterHealthComponent()
{
	SetIsReplicatedByDefault(true);
	MaxShieldHealth = 100;
	ShieldHealth = MaxShieldHealth;
	BodyIndex.Init(0,7);
	if(bSyntySkeleton){SetupSyntySkeleton();}
}

void UCharacterHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetOwnerRole() == ROLE_Authority && !GetIsNPC())
	{
		HealthWidget = CreateWidget<UHealthWidget>(GetOwnerPlayerController(), UHealthWidget::StaticClass());
		if(HealthWidget != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Health Widget Successfully Created"));
		}
	}
	
}

void UCharacterHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(GetIsDead()){return;}
	// Super::TakeDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	if(bHasShield)
	{
		if(ShieldHealth > 0 && Damage > 0)
		{
			ShieldDamage(Damage);
			if(ShieldHealth <= 0)
			{
				//todo Check if works
				OnShieldBreak.Broadcast();
			}
		}
		else
		{
			Super::TakeDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
			// HealthDamage(Damage);
		}
	}
	else
	{
		Super::TakeDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
		// HealthDamage(Damage);
	}
	// if(GetHealth() <= 0.0f && !bDeathOnce)
	// {
	// 	Death();
	// }
	// if(DebugHealthStats)
	// {
	// 	FString String = FString::SanitizeFloat(Health);
	// 	FString Name = GetOwner()->GetName();
	// 	UE_LOG(LogTemp,Warning, TEXT("Health of %s is: %s"), *String, *Name);
	// 	GEngine->AddOnScreenDebugMessage(-1, 24.0f, FColor::Orange, (FString("Health of") + *Name + FString("is") + *String));
	// }
	MyOwner->GetWorldTimerManager().ClearTimer(ShieldTimerHandle);
	//todo Remove 
	// MyOwner->GetWorldTimerManager().SetTimer(HealthTimerHandle, this, &UHealthComponentPlugin::HealthUpdateDelay, 0.001f, false);
	MyOwner->GetWorldTimerManager().SetTimer(ShieldTimerHandle, this, &UCharacterHealthComponent::CheckShieldRegen, 0.01f, true, ShieldTimeToRegen);
	OnHealthAndShieldChanged.Broadcast(this, GetHealth(), MaxHealth, ShieldHealth, MaxShieldHealth, DamageType);
}

void UCharacterHealthComponent::LimbDamage(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone,
	TSubclassOf<UDamageType> in_DamageType)
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
	//todo Add on Injured Body Part
}


void UCharacterHealthComponent::ShieldRegen()
{
	ShieldHealth = FMath::Clamp(ShieldHealth + AmountToRecharge, 0.0f, MaxShieldHealth);
	// OnHealthChanged.Broadcast(this, Health, DefaultHealth, ShieldHealth, DefaultShieldHealth, ShieldDamageType);
	OnHealthAndShieldChanged.Broadcast(this, GetHealth(), MaxHealth, ShieldHealth, MaxShieldHealth, nullptr);
}


void UCharacterHealthComponent::OnRep_Shield()
{
	OnHealthAndShieldChanged.Broadcast(this, GetHealth(), MaxHealth, ShieldHealth, MaxShieldHealth, nullptr);
}

void UCharacterHealthComponent::ShieldDamage(float Damage)
{
	ShieldHealth = FMath::Clamp(ShieldHealth - Damage, 0.0f, MaxShieldHealth);
}

void UCharacterHealthComponent::CheckShieldRegen()
{
	if(ShieldHealth < MaxShieldHealth && !GetIsDead())
	{
		ShieldRegen();
	}
	else
	{
		MyOwner->GetWorldTimerManager().ClearTimer(ShieldTimerHandle);
	}
}

void UCharacterHealthComponent::FaceHealthBarToPlayer()
{
	
}

void UCharacterHealthComponent::SetupSyntySkeleton()
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


void UCharacterHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterHealthComponent, ShieldHealth);

}
