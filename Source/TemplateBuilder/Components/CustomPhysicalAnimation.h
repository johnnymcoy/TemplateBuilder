// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomPhysicalAnimation.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATEBUILDER_API UCustomPhysicalAnimation : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCustomPhysicalAnimation();
    
    UPROPERTY(EditAnywhere)
    class USkeletalMeshComponent* OwnerMesh;
    UFUNCTION(BlueprintCallable)
    void SetupMesh(class USkeletalMeshComponent* MeshToAttach);

    UFUNCTION(BlueprintCallable)
    void HitReaction(FHitResult Hit, float Multiplier = 1);
    UFUNCTION(BlueprintCallable)
    void TogglePhysicalAnimation(bool bTurnOn = true);
    UFUNCTION()
    void SetStrengthMultiplyer(float Strength);
	void SetIsDead(bool bIsDead);
protected:

	virtual void BeginPlay() override;
    class UPhysicalAnimationComponent* PhysicalAnimationRef;



private:
	bool bDead;
    const FName Pelvis = "Pelvis";
    const FName Spine = "Spine_01";

    const FName ProfileStrong = "Strong";
    const FName ProfileWeak = "Weak";
	const FName ProfileRagdoll = "Ragdoll";

    float InterpFloat;
    float HitReactionTimeRemaining = 0;
    float Counter = 0;
    UPROPERTY(EditAnywhere)
    float CounterMultiplier = 0.1;
    UPROPERTY(EditAnywhere)
    float ImpulseMultiplyer = 1000;
    void PhysicalReaction();
	FTimerHandle ReactionLengthTimer;
};
