// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomPhysicalAnimationComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h" 
#include "TimerManager.h"


///
/// TODO: Create different profiles based on how hurt the person is
/// Almost dead - arms and head weak 
//  Have it not effect the legs so much
// Change to gun Impulse
//

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

void UCustomPhysicalAnimationComponent::HitReaction(FHitResult Hit, float Multiplier)
{
	if(!GetIsDead())
	{
		if(HitReactionTimeRemaining <= MaxFloppyTime) //todo Magic number (Max Time can be floppy)
		{
			HitReactionTimeRemaining += 0.1f;
		}
		GetOwner()->GetWorldTimerManager().SetTimer(ReactionLengthTimer, this, &UCustomPhysicalAnimationComponent::PhysicalReaction, 0.01f, true);
		GetOwnerMesh()->SetAllBodiesBelowSimulatePhysics(Pelvis, true, false);
		PhysicalAnimationComponent->ApplyPhysicalAnimationProfileBelow(Pelvis, ProfileStrong, false);
	}
	FName BoneHit = Hit.BoneName;
	if(Hit.BoneName == Pelvis)
	{
		BoneHit = Spine;
	}
	FVector HitVector = (Hit.TraceEnd - Hit.TraceStart);
	HitVector.Normalize(0.0001f);
	// OwnerMesh->AddImpulse((HitVector * ImpulseMultiplyer), BoneHit, true);
	FVector BulletForce = HitVector * Multiplier;
	//    BulletForce.X = (HitVector.X * UKismetMathLibrary::RandomFloatInRange(0.3f, Multiplier));
	// BulletForce.Y = (HitVector.Y * UKismetMathLibrary::RandomFloatInRange(0.3f, Multiplier));
	// BulletForce.Z = (HitVector.Z * UKismetMathLibrary::RandomFloatInRange(0.3f, Multiplier));
	// OwnerMesh->AddImpulse((BulletForce * ImpulseMultiplyer), BoneHit, true);
	
	// HitImpulse.Normalize(0.0001f);
	GetOwnerMesh()->AddImpulse((BulletForce * ImpulseMultiplier), BoneHit, true);
	// OwnerMesh->AddImpulseToAllBodiesBelow((BulletForce * ImpulseMultiplyer), BoneHit, true);

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
		float BlendFloat = UKismetMathLibrary::FMin(HitReactionTimeRemaining * 10, 1);
		UE_LOG(LogTemp,Warning,TEXT("v1.93 %f : TimeRemaining ,BlendFloat: %f "), HitReactionTimeRemaining, BlendFloat);
		GetOwnerMesh()->SetAllBodiesBelowPhysicsBlendWeight(Pelvis, HitReactionTimeRemaining);
	}
}

void UCustomPhysicalAnimationComponent::OwnerDeath()
{
	Super::OwnerDeath();
	HitReactionTimeRemaining = 0.0f;
	GetOwnerMesh()->SetAllBodiesBelowSimulatePhysics(Pelvis, true, true);
	SetStrengthMultiplier(0.0f);
}
