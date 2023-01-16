// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "DlgContext.h"
#include "ALSCharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,meta = (CannotImplementInterfaceInBlueprint))
class UALSCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class TEMPLATEBUILDER_API IALSCharacterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Reaction")
	virtual void AddImpulseEvent(const FHitResult in_Hit, const float in_GunImpulse = 1) = 0;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void BulletDamageEvent(const float in_Damage, const float in_HeadMultiplier, const FName in_HitBone, AController* in_EventInstigator, AActor* in_DamageCauser,
    TSubclassOf<class UDamageType> in_DamageType) = 0;

	UFUNCTION(BlueprintCallable, Category = "Pickups")
	virtual void PickupGunEvent(const FWeaponData in_WeaponData) = 0;

	UFUNCTION(Server, Reliable)
	virtual void DestroyActor(AActor* ActorToDestroy);

	UFUNCTION(NetMulticast, Reliable)
	virtual void DestroyActorOnClient(AActor* ActorToDestroy);

	UFUNCTION(BlueprintCallable, Category = Dialogue)
	virtual void StartDialogue(class UDlgDialogue* Dialogue, const TArray<UObject*>& Participants) = 0;

};
