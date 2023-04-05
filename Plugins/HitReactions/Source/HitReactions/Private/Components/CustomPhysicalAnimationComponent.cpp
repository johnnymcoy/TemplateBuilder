// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomPhysicalAnimationComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h" 
#include "TimerManager.h"
#include "DamageTypes/BulletDamage.h"
#include "GameFramework/Character.h"


/// TODO: Create different profiles based on how hurt the person is
/// Almost dead - arms and head weak 
//  Have it not effect the legs so much

UCustomPhysicalAnimationComponent::UCustomPhysicalAnimationComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UCustomPhysicalAnimationComponent::Setup()
{
	const FTransform Transform;
	PhysicalAnimationComponent = Cast<UPhysicalAnimationComponent>(GetOwner()->AddComponentByClass(UPhysicalAnimationComponent::StaticClass(), false, Transform, false));
	PhysicalAnimationComponent->SetIsReplicated(true);
	if(PhysicalAnimationComponent == nullptr){LogMissingPointer("Physical Animation Component"); return;}
	if(GetOwnerMesh() == nullptr){LogMissingPointer("Mesh in Setup");return;}
	PhysicalAnimationComponent->SetSkeletalMeshComponent(GetOwnerMesh());
}

void UCustomPhysicalAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetOwnerRole() == ROLE_Authority)
	{
		GetOwner()->OnTakePointDamage.AddDynamic(this, &UCustomPhysicalAnimationComponent::TakePointDamage);
	}
}

void UCustomPhysicalAnimationComponent::TogglePhysicalAnimation(bool bTurnOn)
{
	GetOwnerMesh()->SetAllBodiesBelowSimulatePhysics(Pelvis, bTurnOn, false);
	if(bTurnOn)
	{
		if(PhysicalAnimationComponent == nullptr){LogMissingPointer("Physical Animation Component"); return;}
		PhysicalAnimationComponent->ApplyPhysicalAnimationProfileBelow(Pelvis, ProfileWeak);
	}
}

void UCustomPhysicalAnimationComponent::SetStrengthMultiplier(float Strength)
{
	if(PhysicalAnimationComponent == nullptr){LogMissingPointer("Physical Animation Component"); return;}
	GetOwner()->GetWorldTimerManager().ClearTimer(ReactionLengthTimer);
	PhysicalAnimationComponent->SetStrengthMultiplyer(Strength);
}


//////////////////-		Point Damage				////
	
void UCustomPhysicalAnimationComponent::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
                                                        FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                                                        const UDamageType* DamageType, AActor* DamageCauser)
{
	if(PhysicalAnimationComponent == nullptr){LogMissingPointer("Physical Animation Component"); return;}

	
	// todo Multiply depending on DamageType
	//? WIP
	float DamageTypeMultiplier = 1.0f;
	// switch (DamageType)
	// {
	// default:
	// 	DamageTypeMultiplier = 1.0f;
	// 	break;
	// case(UBulletDamage::StaticClass()):
	// 	UE_LOG(LogTemp,Warning,TEXT("Bullet Damage"));
	// 	break;
	// }

	
	// FVector HitVector = (in_Hit.TraceEnd - in_Hit.TraceStart);
	// HitVector.Normalize(0.0001f);
	// HitVector *= in_GunImpulse;
	//todo: get proper number, also add in ragdoll if high enough
	//! Remove, Plays animation over any reaction.. 
	// if(Damage > LaunchCharacterThreshold)
	// {
	// 	ACharacter* OwningCharacter = Cast<ACharacter>(GetOwner());
	// 	if(OwningCharacter == nullptr){LogMissingPointer("Owner Character Cast Failed");return;}
	// 	OwningCharacter->LaunchCharacter(ShotFromDirection,true,true);
	// }
	LastHitBone = BoneName;
	LastHitVector = ShotFromDirection;
	LastMultiplier = (ImpulseMultiplier * Damage * DamageTypeMultiplier);
	HitReaction(BoneName, ShotFromDirection, (ImpulseMultiplier * Damage * DamageTypeMultiplier));
}


void UCustomPhysicalAnimationComponent::HitReaction(FName BoneHit, FVector HitVector, float Multiplier)
{
	if(!GetIsDead())
	{
		if(HitReactionTimeRemaining <= MaxFloppyTime) //- (Max Time can be floppy)
		{
			HitReactionTimeRemaining += 0.1f;
		}
		GetOwner()->GetWorldTimerManager().SetTimer(ReactionLengthTimer, this, &UCustomPhysicalAnimationComponent::PhysicalReaction, 0.01f, true);
		GetOwnerMesh()->SetAllBodiesBelowSimulatePhysics(Pelvis, true, false);
		PhysicalAnimationComponent->ApplyPhysicalAnimationProfileBelow(Pelvis, ProfileStrong, false);
	}
	if(BoneHit == Pelvis)
	{
		BoneHit = Spine;
	}
	const FVector BulletForce = HitVector * Multiplier;
	GetOwnerMesh()->AddImpulse((BulletForce * ImpulseMultiplier), BoneHit, true);
	GetOwnerMesh()->AddImpulseToAllBodiesBelow(((BulletForce * ImpulseMultiplier) / 2), BoneHit, true);
}

// todo Continue with changing above value of how long they should be floppy
// todo continue with changing the Blend weight so it more naturally changes back to non floppy
// the problem is probably in the *10, change and log the numbers before going into the blend
void UCustomPhysicalAnimationComponent::PhysicalReaction()
{
	Counter = (GetWorld()->GetDeltaSeconds() * CounterMultiplier);
	HitReactionTimeRemaining = (FMath::FInterpTo(HitReactionTimeRemaining, 0.0f, 0.0f, 1.0f) - Counter);
	// UE_LOG(LogTemp,Warning,TEXT(" %f : TimeRemaining 1.91 ,Counter: %f "), HitReactionTimeRemaining, Counter);
	if(HitReactionTimeRemaining <= 0)
	{
		HitReactionTimeRemaining = 0;
		GetOwnerMesh()->SetAllBodiesBelowSimulatePhysics(Pelvis, false, true);
		GetOwner()->GetWorldTimerManager().ClearTimer(ReactionLengthTimer);
	}
	else
	{
		const float BlendFloat = UKismetMathLibrary::FMin((HitReactionTimeRemaining * BlendTimeMultiplier), 1);
		if(bDebuggingMode){UE_LOG(LogTemp,Warning,TEXT("v1.94 %f : TimeRemaining ,BlendFloat: %f "), HitReactionTimeRemaining, BlendFloat);}
		GetOwnerMesh()->SetAllBodiesBelowPhysicsBlendWeight(Pelvis, BlendFloat);
	}
}

void UCustomPhysicalAnimationComponent::OwnerDeath()
{
	Super::OwnerDeath();
	HitReaction(LastHitBone, LastHitVector, LastMultiplier);
	HitReactionTimeRemaining = 0.0f;
	GetOwnerMesh()->SetAllBodiesBelowSimulatePhysics(Pelvis, true, true);
	SetStrengthMultiplier(0.0f);
}
