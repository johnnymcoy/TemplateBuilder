// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseInterface.generated.h"

UINTERFACE()
class UBaseInterface : public UInterface
{
	GENERATED_BODY()
};

class BASEHELPERS_API IBaseInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(Server, Reliable)
	virtual void DestroyActor(AActor* ActorToDestroy);

	UFUNCTION(NetMulticast, Reliable)
	virtual void DestroyActorOnClient(AActor* ActorToDestroy);

};
