// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DismembermentComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATEBUILDER_API UDismembermentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDismembermentComponent();

protected:
	virtual void BeginPlay() override;


		
};
