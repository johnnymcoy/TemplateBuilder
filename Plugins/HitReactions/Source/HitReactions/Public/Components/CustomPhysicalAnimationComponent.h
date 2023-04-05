// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterComponent.h"
#include "CustomPhysicalAnimationComponent.generated.h"

UCLASS()
class HITREACTIONS_API UCustomPhysicalAnimationComponent : public UCharacterComponent
{
	GENERATED_BODY()
	
public:	
	UCustomPhysicalAnimationComponent();
	
	UFUNCTION(BlueprintCallable, Category="Physical Animation")
	void Setup();

	UFUNCTION(BlueprintCallable, Category="Physical Animation")
	void HitReaction(FName BoneHit, FVector HitVector, float Multiplier = 1);
	UFUNCTION(BlueprintCallable, Category="Physical Animation")
	void TogglePhysicalAnimation(bool bTurnOn = true);
	UFUNCTION()
	void SetStrengthMultiplier(float Strength);

	virtual void OwnerDeath() override;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void TakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	
	UPROPERTY()
	class UPhysicalAnimationComponent* PhysicalAnimationComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physical Animation")
	float MaxFloppyTime = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physical Animation")
	float CounterMultiplier = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physical Animation")
	float ImpulseMultiplier = 1000.0f;
	//- How much Damage Has to be done for it to move that character // 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physical Animation")
	float BlendTimeMultiplier = 10.0f;

	
private:
	void PhysicalReaction();



	const FName Pelvis = "Pelvis";
	const FName Spine = "Spine_01";

	const FName ProfileStrong = "Strong";
	const FName ProfileWeak = "Weak";
	const FName ProfileRagdoll = "Ragdoll";

	float InterpFloat;
	float HitReactionTimeRemaining = 0.0f;
	float Counter = 0.0f;
	FTimerHandle ReactionLengthTimer;

	FVector LastHitVector = FVector::ZeroVector;
	FName LastHitBone = Spine;
	float LastMultiplier = 1.0f;
};
