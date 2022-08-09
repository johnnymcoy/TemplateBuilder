// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"

#include "BrainComponent.h"
#include "TemplateBuilder/Characters/ALSCustomAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


void AAIControllerBase::BeginPlay() 
{
    Super::BeginPlay();

    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
    }
}

void AAIControllerBase::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);
    
}

void AAIControllerBase::Death()
{
    GetBrainComponent()->StopLogic("Death");
    SetActorTickEnabled(false);
    GetBrainComponent()->Cleanup();
    UnPossess();
}

bool AAIControllerBase::IsDead() const
{
    AALSCustomAICharacter* ControlledCharacter = Cast<AALSCustomAICharacter>(GetPawn());
    if(ControlledCharacter)
    {
        return ControlledCharacter->IsDead();
    }
    return true;
}
