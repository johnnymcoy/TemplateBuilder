// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPhysicalAnimation.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Kismet/KismetMathLibrary.h" 
#include "TimerManager.h"

///
/// TODO: Create different profiles based on how hurt the person is
/// Almost dead - arms and head weak 
//  Have it not effect the legs so much
// Change to gun Impulse
//
UCustomPhysicalAnimation::UCustomPhysicalAnimation()
{
    PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UCustomPhysicalAnimation::BeginPlay()
{
	Super::BeginPlay();
    const FTransform Transform;
    PhysicalAnimationRef = Cast<UPhysicalAnimationComponent>(GetOwner()->AddComponentByClass(UPhysicalAnimationComponent::StaticClass(), false, Transform, false));
    PhysicalAnimationRef->SetIsReplicated(true);
}

void UCustomPhysicalAnimation::SetupMesh(USkeletalMeshComponent* MeshToAttach)
{
    OwnerMesh = MeshToAttach;
    PhysicalAnimationRef->SetSkeletalMeshComponent(OwnerMesh);
}

void UCustomPhysicalAnimation::TogglePhysicalAnimation(bool bTurnOn)
{
    OwnerMesh->SetAllBodiesBelowSimulatePhysics(Pelvis, bTurnOn, false);
    if(bTurnOn)
    {
        PhysicalAnimationRef->ApplyPhysicalAnimationProfileBelow(Pelvis, ProfileWeak);
    }
}

void UCustomPhysicalAnimation::HitReaction(FHitResult Hit, float Multiplier)
{
    if(!bDead)
    {
        if(HitReactionTimeRemaining <= 1.5) //todo Magic number (Max Time can be floppy)
        {
            HitReactionTimeRemaining += 0.1f;
        }
        GetOwner()->GetWorldTimerManager().SetTimer(ReactionLengthTimer, this, &UCustomPhysicalAnimation::PhysicalReaction, 0.01f, true);
        OwnerMesh->SetAllBodiesBelowSimulatePhysics(Pelvis, true, false);
        PhysicalAnimationRef->ApplyPhysicalAnimationProfileBelow(Pelvis, ProfileStrong, false);
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
    OwnerMesh->AddImpulse((BulletForce * ImpulseMultiplyer), BoneHit, true);
    // OwnerMesh->AddImpulseToAllBodiesBelow((BulletForce * ImpulseMultiplyer), BoneHit, true);
}
// todo Continue with changing above value of how long they should be floppy
// todo continue with changing the Blend weight so it more naturally changes back to non floppy
// the problem is probably in the *10, change and log the numbers before going into the blend
void UCustomPhysicalAnimation::PhysicalReaction()
{
    Counter = (GetWorld()->GetDeltaSeconds() * CounterMultiplier);
    HitReactionTimeRemaining = (FMath::FInterpTo(HitReactionTimeRemaining, 0.0f, 0.0f, 1.0f) - Counter);
    // UE_LOG(LogTemp,Warning,TEXT(" %f : TimeRemaining 1.91 ,Counter: %f "), HitReactionTimeRemaining, Counter);
    if(HitReactionTimeRemaining <= 0)
    {
        HitReactionTimeRemaining = 0;
        OwnerMesh->SetAllBodiesBelowSimulatePhysics(Pelvis, false, true);
        GetOwner()->GetWorldTimerManager().ClearTimer(ReactionLengthTimer);
    }
    else
    {
        float BlendFloat = UKismetMathLibrary::FMin(HitReactionTimeRemaining * 10, 1);
        UE_LOG(LogTemp,Warning,TEXT("v1.93 %f : TimeRemaining ,BlendFloat: %f "), HitReactionTimeRemaining, BlendFloat);
        OwnerMesh->SetAllBodiesBelowPhysicsBlendWeight(Pelvis, HitReactionTimeRemaining);
    }
}

void UCustomPhysicalAnimation::SetStrengthMultiplyer(float Strength)
{
    GetOwner()->GetWorldTimerManager().ClearTimer(ReactionLengthTimer);
    PhysicalAnimationRef->SetStrengthMultiplyer(Strength);
}

void UCustomPhysicalAnimation::SetIsDead(bool bIsDead)
{
    bDead = bIsDead;
    //
    HitReactionTimeRemaining = 0;
    OwnerMesh->SetAllBodiesBelowSimulatePhysics(Pelvis, true, true);
    //
    SetStrengthMultiplyer(0);
}

