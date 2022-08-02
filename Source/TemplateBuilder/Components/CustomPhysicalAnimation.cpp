// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPhysicalAnimation.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"

UCustomPhysicalAnimation::UCustomPhysicalAnimation()
{
    PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomPhysicalAnimation::BeginPlay()
{
	Super::BeginPlay();
    FTransform Transform;
    PhysicalAnimationRef = Cast<UPhysicalAnimationComponent>(GetOwner()->AddComponentByClass(UPhysicalAnimationComponent::StaticClass(), false, Transform, false));
}

void UCustomPhysicalAnimation::SetupMesh(USkeletalMeshComponent* MeshToAttach)
{
    OwnerMesh = MeshToAttach;
    PhysicalAnimationRef->SetSkeletalMeshComponent(OwnerMesh);
}

void UCustomPhysicalAnimation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCustomPhysicalAnimation::TogglePhysicalAnimation(bool bTurnOn)
{
    OwnerMesh->SetAllBodiesBelowSimulatePhysics(Spine, bTurnOn, false);
    PhysicalAnimationRef->ApplyPhysicalAnimationProfileBelow(Spine, ProfileStrong);
}


void UCustomPhysicalAnimation::HitReaction(FHitResult Hit)
{
    PrimaryComponentTick.SetTickFunctionEnable(true);
    HitReactionTimeRemaining += 2;
    TogglePhysicalAnimation();
    // PhysicalAnimationRef->ApplyPhysicalAnimationProfileBelow();
}
