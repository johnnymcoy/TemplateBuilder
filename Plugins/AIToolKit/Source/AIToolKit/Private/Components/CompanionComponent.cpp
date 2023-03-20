// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CompanionComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

UCompanionComponent::UCompanionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCompanionComponent::BeginPlay()
{
	Super::BeginPlay();
	//- Do we have a master predefined //
	bHasMaster = (MasterActor != nullptr);
}



void UCompanionComponent::SetMasterActor(AActor* Master)
{
	if(Master != nullptr)
	{
		MasterActor = Master;
		bHasMaster = true;
		if(BlackboardComponent != nullptr)
		{
			BlackboardComponent->SetValueAsObject(FName("Master"), MasterActor);
		}
		else
		{
			SetupBlackboard();
		}
	}
}

void UCompanionComponent::SetupBlackboard()
{
	if(GetOwnerAIController())
	{
		BlackboardComponent = GetOwnerAIController()->GetBlackboardComponent();
		if(bHasMaster && BlackboardComponent != nullptr)
		{
			BlackboardComponent->SetValueAsObject(FName("Master"), MasterActor);
		}
	}
}

void UCompanionComponent::UpdateCompanionState(ECompanionState NewState)
{
	CompanionState = NewState;
	if(BlackboardComponent != nullptr)
	{
		//todo Check this conversion works 
		// uint8 byte = (uint8)CompanionState;
		BlackboardComponent->SetValueAsEnum(FName("State"), (uint8)CompanionState);
	}
}

void UCompanionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


//-			Interface Functions			-//

void UCompanionComponent::Follow()
{
	UpdateCompanionState(ECompanionState::Follow);
	if(BlackboardComponent == nullptr){LogMissingPointer("Blackboard");}
}

void UCompanionComponent::Fetch(UPrimitiveComponent* ComponentToGrab)
{
	if(ComponentToGrab != nullptr)
	{
		ObjectToGrab = ComponentToGrab;
	}
	UpdateCompanionState(ECompanionState::Fetch);
}

void UCompanionComponent::StartCombat(AActor* TargetEnemy)
{
	if(TargetEnemy != nullptr)
	{
		TargetActor = TargetEnemy;
	}
	UpdateCompanionState(ECompanionState::Combat);
	if(BlackboardComponent != nullptr)
	{
		BlackboardComponent->SetValueAsObject(FName("AttackTarget"), TargetActor);
	}
}

void UCompanionComponent::Wait(FVector TargetLocation)
{
	UpdateCompanionState(ECompanionState::Follow);
	UpdateCompanionState(ECompanionState::Wait);
	if(BlackboardComponent != nullptr)
	{
		BlackboardComponent->SetValueAsVector(FName("WaitingLocation"), TargetLocation);
	}

}

void UCompanionComponent::GrabObject()
{
	//todo Move to Character?
	if(bUseGrabAnimation)
	{
		Server_PlayMontageAnimation(GrabAnimMontage, 1.0f, EMontagePlayReturnType::Duration, 0.0f, true);
		//todo Bug test animation Duration time 
	}		GetWorld()->GetTimerManager().SetTimer(GrabTimerHandle, this, &UCompanionComponent::OnGrabObject, 0.01f, false, LastAnimationDuration);

}

void UCompanionComponent::OnGrabObject()
{
	
}

// Todo Attack function
// Todo move to Character Pawn
void UCompanionComponent::Attack()
{
	if(AttackAnimMontage == nullptr){LogMissingPointer("Attack Montage");return;}

	Server_PlayMontageAnimation(AttackAnimMontage, 1.0f, EMontagePlayReturnType::Duration, 0.0f, true);
	//! ApplyDamage-> get return?
	//if(enemy-dead)
	UpdateCompanionState(ECompanionState::Follow);
	
}
