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
}

void UCharacterHealthComponent::SetHasShield(bool in_bHasShield, float in_MaxAmount, float in_ShieldTimeToRegen, float in_AmountToRecharge)
{
	bHasShield = in_bHasShield;
	MaxShieldHealth = in_MaxAmount;
	ShieldTimeToRegen = in_ShieldTimeToRegen;
	AmountToRecharge = in_AmountToRecharge;
}

void UCharacterHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	// todo Check Dynamic Multiplayer
	if(GetOwnerRole() == ROLE_Authority)
	{
		MyOwner->OnTakePointDamage.AddDynamic(this, &UCharacterHealthComponent::TakePointDamage);
	}
	// todo
	// if(GetOwnerRole() == ROLE_Authority && !GetIsNPC())
	// {
	// 	//Todo this
	// 	HealthWidget = CreateWidget<UHealthWidget>(GetOwnerPlayerController(), UHealthWidget::StaticClass());
	// 	if(HealthWidget != nullptr)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Health Widget Successfully Created"));
	// 	}
	// }
	if(bSyntySkeleton){SetupSyntySkeleton();}
	if(bUseCustomBones)
	{
	// Head.Bones = HeadBones;
	// Spine.Bones = SpineBones;
	// RightArm.Bones = RightArmBones;
	// LeftArm.Bones = LeftArmBones;
	// RightLeg.Bones = RightLegBones;
	// LeftLeg.Bones = LeftLegBones;
	// RightHand.Bones = RightHandBones;
	// LeftHand.Bones = LeftHandBones;
	}
	SetupLimbs();
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
	MyOwner->GetWorldTimerManager().ClearTimer(ShieldTimerHandle);
	MyOwner->GetWorldTimerManager().SetTimer(ShieldTimerHandle, this, &UCharacterHealthComponent::CheckShieldRegen, 0.01f, true, ShieldTimeToRegen);
	OnHealthAndShieldChanged.Broadcast(this, GetHealth(), MaxHealth, ShieldHealth, MaxShieldHealth, DamageType);
}

void UCharacterHealthComponent::TakePointDamage(AActor* DamagedActor, float Damage,
	AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName,
	FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	// DamageType
	if(BoneName == FName("head"))
	{
		//todo headshot damage
		float HeadshotDamage = (Damage * Damage);
		// UGameplayStatics::ApplyDamage(this, HeadshotDamage, in_EventInstigator, in_DamageCauser, in_DamageType);
	}
	LimbDamage(Damage, BoneName, DamageType);
}

void UCharacterHealthComponent::LimbDamage(const float in_Damage, const FName in_HitBone,
                                           const UDamageType* in_DamageType)
{
	for (FBodyPart& BoneData : BodyParts)
	{
		for(int i = 0; i < BoneData.Bones.Num(); i++)
		{
			if(in_HitBone == BoneData.Bones[i])
			{
				BoneData.BodyPartHealth -= (in_Damage);
				UE_LOG(LogTemp,Warning,TEXT("%s Hit! %f Damage"), *in_HitBone.ToString(), BoneData.BodyPartHealth);
				if(BoneData.BodyPartHealth <= BodyPartInjuredHealthLevel)
				{
					InjuredBodyPart(BoneData.BodyPartName);
				}
				return;
			}
		}
	}
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

void UCharacterHealthComponent::InjuredBodyPart(FName BodyPart)
{
	OnInjuredBodyPart.Broadcast(BodyPart);
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
	//todo
}

void UCharacterHealthComponent::SetupSyntySkeleton()
{
	Head.Bones.Add(FName("head"));
	Head.Bones.Add(FName("eyes"));
	Head.Bones.Add(FName("eyebrows"));
	Head.Bones.Add(FName("neck_01"));
	Spine.Bones.Add(FName("spine_01"));
	Spine.Bones.Add(FName("spine_02"));
	Spine.Bones.Add(FName("spine_03"));
	Spine.Bones.Add(FName("Pelvis"));
	RightArm.Bones.Add(FName("clavicle_r"));
	RightArm.Bones.Add(FName("UpperArm_R"));
	RightArm.Bones.Add(FName("lowerarm_r"));
	LeftArm.Bones.Add(FName("clavicle_l"));
	LeftArm.Bones.Add(FName("UpperArm_L"));
	LeftArm.Bones.Add(FName("lowerarm_l"));
	RightLeg.Bones.Add(FName("Thigh_R"));
	RightLeg.Bones.Add(FName("calf_r"));
	RightLeg.Bones.Add(FName("Foot_R"));
	RightLeg.Bones.Add(FName("ball_r"));
	RightLeg.Bones.Add(FName("toes_r"));
	LeftLeg.Bones.Add(FName("Thigh_L"));
	LeftLeg.Bones.Add(FName("calf_l"));
	LeftLeg.Bones.Add(FName("Foot_L"));
	LeftLeg.Bones.Add(FName("ball_l"));
	LeftLeg.Bones.Add(FName("toes_l"));
	LeftHand.Bones.Add(FName("Hand_L"));
	LeftHand.Bones.Add(FName("thumb_01_l"));
	LeftHand.Bones.Add(FName("thumb_02_l"));
	LeftHand.Bones.Add(FName("thumb_03_l"));
	LeftHand.Bones.Add(FName("indexFinger_01_l"));
	LeftHand.Bones.Add(FName("indexFinger_02_l"));
	LeftHand.Bones.Add(FName("indexFinger_03_l"));
	LeftHand.Bones.Add(FName("indexFinger_04_l"));
	LeftHand.Bones.Add(FName("finger_01_l"));
	LeftHand.Bones.Add(FName("finger_02_l"));
	RightHand.Bones.Add(FName("hand_r"));
	RightHand.Bones.Add(FName("thumb_01_r"));
	RightHand.Bones.Add(FName("thumb_02_r"));
	RightHand.Bones.Add(FName("thumb_03_r"));
	RightHand.Bones.Add(FName("indexFinger_01_r"));
	RightHand.Bones.Add(FName("indexFinger_02_r"));
	RightHand.Bones.Add(FName("indexFinger_03_r"));
	RightHand.Bones.Add(FName("indexFinger_04_r"));
	RightHand.Bones.Add(FName("finger_01_r"));
	RightHand.Bones.Add(FName("finger_02_r"));
}

void UCharacterHealthComponent::SetupLimbs()
{
	Head.BodyPartName = FName("Head");
	Spine.BodyPartName = FName("Spine");
	RightArm.BodyPartName = FName("Right Arm");
	LeftArm.BodyPartName = FName("LeftA rm");
	RightLeg.BodyPartName = FName("Right Leg");
	LeftLeg.BodyPartName = FName("Left Leg");
	LeftHand.BodyPartName = FName("Left Hand");
	RightHand.BodyPartName = FName("Right Hand");
	Head.BodyPartHealth = BodyPartMaxHealth;
	Spine.BodyPartHealth = BodyPartMaxHealth;
	RightArm.BodyPartHealth = BodyPartMaxHealth;
	LeftArm.BodyPartHealth = BodyPartMaxHealth;
	RightLeg.BodyPartHealth = BodyPartMaxHealth;
	LeftLeg.BodyPartHealth = BodyPartMaxHealth;
	LeftHand.BodyPartHealth = BodyPartMaxHealth;
	RightHand.BodyPartHealth = BodyPartMaxHealth;
	BodyParts.Add(Head);
	BodyParts.Add(Spine);
	BodyParts.Add(RightArm);
	BodyParts.Add(LeftArm);
	BodyParts.Add(RightLeg);
	BodyParts.Add(LeftLeg);
	BodyParts.Add(LeftHand);
	BodyParts.Add(RightHand);
}


void UCharacterHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterHealthComponent, ShieldHealth);

}
