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
    
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    UPROPERTY(EditAnywhere)
    class USkeletalMeshComponent* OwnerMesh;
    UFUNCTION(BlueprintCallable)
    void SetupMesh(class USkeletalMeshComponent* MeshToAttach);

    UFUNCTION(BlueprintCallable)
    void HitReaction(FHitResult Hit);
protected:

	virtual void BeginPlay() override;
    UFUNCTION(BlueprintCallable)
    void TogglePhysicalAnimation(bool bTurnOn = true);
    class UPhysicalAnimationComponent* PhysicalAnimationRef;



private:
    const FName Pelvis = "Pelvis";
    const FName Spine = "Spine_01";

    const FName ProfileStrong = "Strong";
    // const FName ProfileStrong = "Strong";

    float HitReactionTimeRemaining;

		
};
