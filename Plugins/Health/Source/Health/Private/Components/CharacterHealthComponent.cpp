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

void UCharacterHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetOwnerRole() == ROLE_Authority && !GetIsNPC())
	{
		//Todo this
		MyOwner->OnTakePointDamage.AddDynamic(this, &UCharacterHealthComponent::TakePointDamage);

		HealthWidget = CreateWidget<UHealthWidget>(GetOwnerPlayerController(), UHealthWidget::StaticClass());
		if(HealthWidget != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Health Widget Successfully Created"));
		}
	}
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

void UCharacterHealthComponent::TakePointDamage(AActor* DamagedActor, float Damage,
	AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName,
	FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	// DamageType
	UE_LOG(LogTemp,Warning,TEXT("PointDamage %f"), Damage);
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
	// v3.0
	// int t = 0;
	for (FBodyPart& BoneData : BodyParts)
	{
		// t++;
		// UE_LOG(LogTemp,Warning,TEXT("First Index %i, %i"), t, BoneData.Bones.Num());
		for(int i = 0; i < BoneData.Bones.Num(); i++)
		{
			// UE_LOG(LogTemp,Warning,TEXT("Second Loop Index %i, %s"), i, *BoneData.Bones[i].ToString());
			if(in_HitBone == BoneData.Bones[i])
			{
				BoneData.BodyPartHealth -= (in_Damage);
				
				OnInjuredBodyPart.Broadcast(BoneData.BodyPartName);
				UE_LOG(LogTemp,Warning,TEXT("%s Hit! %f Damage"), *in_HitBone.ToString(), BoneData.BodyPartHealth);
				return;
			}
		}
	}

	// V 2.1
	//todo Double for loop, only problem is unreal doesn't support Array in Array
	// TArray<TArray<FName>> BodyParts = { HeadBones, RightArmBones, LeftArmBones, RightLegBones, LeftLegBones, RightHandBones, LeftHandBones };
	// UE_LOG(LogTemp,Warning,TEXT("Bone: %s , HeadBone: %s From Character Health Component"), *in_HitBone.ToString(), *HeadBones[0].ToString());
	// for(int i = 0; i < BodyParts.Num(); i++)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Start For loop: %i, Number: %i "), i, BodyParts[i].Num());
	// 	for(int j = 0; j < BodyParts[i].Num(); j++)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Second For loop: %i , %i"), i,j);
	// 		if(in_HitBone == BodyParts[i][j])
	// 		{
	// 			BodyIndex[i] = BodyIndex[i] + (in_Damage);
	// 			UE_LOG(LogTemp, Warning, TEXT("HeadHitCounter: %f"), BodyIndex[0]);
	// 			break;
	// 		}
	// 	}
	// }

	// v1.1
	// // UE_LOG(LogTemp, Warning, TEXT("HitBOne: %s"), *in_HitBone.ToString());
	// for(int i = 0; i < HeadBones.Num(); i++)
	// {
	// 	if(in_HitBone == HeadBones[i])
	// 	{
	// 		BodyIndex[0] = BodyIndex[0] + (in_Damage);
	// 		// UE_LOG(LogTemp, Warning, TEXT("HeadHitCounter: %f"), BodyIndex[0]);
	// 	}
	// }
	// for(int i = 0; i < RightArmBones.Num(); i++)
	// {
	// 	if(in_HitBone == RightArmBones[i])
	// 	{
	// 		BodyIndex[1] = BodyIndex[1] + (in_Damage);
	// 		// UE_LOG(LogTemp, Warning, TEXT("RightArmBones: %f"), BodyIndex[1]);
	// 	}
	// }
	// for(int i = 0; i < LeftArmBones.Num(); i++)
	// {
	// 	if(in_HitBone == LeftArmBones[i])
	// 	{
	// 		BodyIndex[2] = BodyIndex[2] + (in_Damage);
	// 		// UE_LOG(LogTemp, Warning, TEXT("LeftArmBones: %f"), BodyIndex[2]);
	// 	}
	// }
	// for(int i = 0; i < RightLegBones.Num(); i++)
	// {
	// 	if(in_HitBone == RightLegBones[i])
	// 	{
	// 		BodyIndex[3] = BodyIndex[3] + (in_Damage);
	// 		// UE_LOG(LogTemp, Warning, TEXT("RightLegBones: %f"), BodyIndex[3]);
	//
	// 	}
	// }
	// for(int i = 0; i < LeftLegBones.Num(); i++)
	// {
	// 	if(in_HitBone == LeftLegBones[i])
	// 	{
	// 		BodyIndex[4] = BodyIndex[4] + (in_Damage);
	// 		// UE_LOG(LogTemp, Warning, TEXT("LeftLegBones: %f"), BodyIndex[4]);
	// 	}
	// }
	// for(int i = 0; i < RightHandBones.Num(); i++)
	// {
	// 	if(in_HitBone == RightHandBones[i])
	// 	{
	// 		BodyIndex[5] = BodyIndex[5] + (in_Damage);
	// 		// UE_LOG(LogTemp, Warning, TEXT("RightHandBones: %f"), BodyIndex[5]);
	// 	}
	// }
	// for(int i = 0; i < LeftHandBones.Num(); i++)
	// {
	// 	if(in_HitBone == LeftHandBones[i])
	// 	{
	// 		BodyIndex[6] = BodyIndex[6] + (in_Damage);
	// 		// UE_LOG(LogTemp, Warning, TEXT("LeftHandBones: %f"), BodyIndex[6]);
	// 	}
	// }
	// //todo Add on Injured Body Part
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
	//todo
}

void UCharacterHealthComponent::SetupSyntySkeleton()
{
	// V 2.0

	// BodyParts[0].Add(FName("head"));
	// BodyParts[0].Add(FName("eyes"));
	// BodyParts[0].Add(FName("eyebrows"));
	// BodyParts[1].Add(FName("clavicle_r"));
	// BodyParts[1].Add(FName("UpperArm_R"));
	// BodyParts[1].Add(FName("lowerarm_r"));
	// BodyParts[2].Add(FName("clavicle_l"));
	// BodyParts[2].Add(FName("UpperArm_L"));
	// BodyParts[2].Add(FName("lowerarm_l"));
	// BodyParts[3].Add(FName("Thigh_R"));
	// BodyParts[3].Add(FName("calf_r"));
	// BodyParts[3].Add(FName("Foot_R"));
	// BodyParts[3].Add(FName("ball_r"));
	// BodyParts[3].Add(FName("toes_r"));
	// BodyParts[4].Add(FName("Thigh_L"));
	// BodyParts[4].Add(FName("calf_l"));
	// BodyParts[4].Add(FName("Foot_L"));
	// BodyParts[4].Add(FName("ball_l"));
	// BodyParts[4].Add(FName("toes_l"));
	// BodyParts[5].Add(FName("Hand_L"));
	// BodyParts[5].Add(FName("thumb_01_l"));
	// BodyParts[5].Add(FName("thumb_02_l"));
	// BodyParts[5].Add(FName("thumb_03_l"));
	// BodyParts[5].Add(FName("indexFinger_01_l"));
	// BodyParts[5].Add(FName("indexFinger_02_l"));
	// BodyParts[5].Add(FName("indexFinger_03_l"));
	// BodyParts[5].Add(FName("indexFinger_04_l"));
	// BodyParts[5].Add(FName("finger_01_l"));
	// BodyParts[5].Add(FName("finger_02_l"));
	// BodyParts[6].Add(FName("hand_r"));
	// BodyParts[6].Add(FName("thumb_01_r"));
	// BodyParts[6].Add(FName("thumb_02_r"));
	// BodyParts[6].Add(FName("thumb_03_r"));
	// BodyParts[6].Add(FName("indexFinger_01_r"));
	// BodyParts[6].Add(FName("indexFinger_02_r"));
	// BodyParts[6].Add(FName("indexFinger_03_r"));
	// BodyParts[6].Add(FName("indexFinger_04_r"));
	// BodyParts[6].Add(FName("finger_01_r"));
	// BodyParts[6].Add(FName("finger_02_r"));

	// V 3.0
	// Head.BodyPartName = FName("Head");

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
	
	// V 1.1 
	// HeadBones.Add(FName("head"));
	// HeadBones.Add(FName("eyes"));
	// HeadBones.Add(FName("eyebrows"));
	// RightArmBones.Add(FName("clavicle_r"));
	// RightArmBones.Add(FName("UpperArm_R"));
	// RightArmBones.Add(FName("lowerarm_r"));
	// LeftArmBones.Add(FName("clavicle_l"));
	// LeftArmBones.Add(FName("UpperArm_L"));
	// LeftArmBones.Add(FName("lowerarm_l"));
	// RightLegBones.Add(FName("Thigh_R"));
	// RightLegBones.Add(FName("calf_r"));
	// RightLegBones.Add(FName("Foot_R"));
	// RightLegBones.Add(FName("ball_r"));
	// RightLegBones.Add(FName("toes_r"));
	// LeftLegBones.Add(FName("Thigh_L"));
	// LeftLegBones.Add(FName("calf_l"));
	// LeftLegBones.Add(FName("Foot_L"));
	// LeftLegBones.Add(FName("ball_l"));
	// LeftLegBones.Add(FName("toes_l"));
	// LeftHandBones.Add(FName("Hand_L"));
	// LeftHandBones.Add(FName("thumb_01_l"));
	// LeftHandBones.Add(FName("thumb_02_l"));
	// LeftHandBones.Add(FName("thumb_03_l"));
	// LeftHandBones.Add(FName("indexFinger_01_l"));
	// LeftHandBones.Add(FName("indexFinger_02_l"));
	// LeftHandBones.Add(FName("indexFinger_03_l"));
	// LeftHandBones.Add(FName("indexFinger_04_l"));
	// LeftHandBones.Add(FName("finger_01_l"));
	// LeftHandBones.Add(FName("finger_02_l"));
	// RightHandBones.Add(FName("hand_r"));
	// RightHandBones.Add(FName("thumb_01_r"));
	// RightHandBones.Add(FName("thumb_02_r"));
	// RightHandBones.Add(FName("thumb_03_r"));
	// RightHandBones.Add(FName("indexFinger_01_r"));
	// RightHandBones.Add(FName("indexFinger_02_r"));
	// RightHandBones.Add(FName("indexFinger_03_r"));
	// RightHandBones.Add(FName("indexFinger_04_r"));
	// RightHandBones.Add(FName("finger_01_r"));
	// RightHandBones.Add(FName("finger_02_r"));
}

void UCharacterHealthComponent::SetupLimbs()
{
	Head.BodyPartName = FName("Head");
	Spine.BodyPartName = FName("Spine");
	RightArm.BodyPartName = FName("RightArm");
	LeftArm.BodyPartName = FName("LeftArm");
	RightLeg.BodyPartName = FName("RightLeg");
	LeftLeg.BodyPartName = FName("LeftLeg");
	LeftHand.BodyPartName = FName("LeftHand");
	RightHand.BodyPartName = FName("RightHand");
	Head.BodyPartHealth = 100.0f;
	Spine.BodyPartHealth = 100.0f;
	RightArm.BodyPartHealth = 100.0f;
	LeftArm.BodyPartHealth = 100.0f;
	RightLeg.BodyPartHealth = 100.0f;
	LeftLeg.BodyPartHealth = 100.0f;
	LeftHand.BodyPartHealth = 100.0f;
	RightHand.BodyPartHealth = 100.0f;
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
