// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterComponent.h"
#include "Components/Widget.h"

UCharacterComponent::UCharacterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCharacterComponent::SetupComponent(USkeletalMeshComponent* SkeletalMesh, UAnimInstance* AnimationInstance,
                                         AController* Controller, const bool bNPC, const bool bDead)
{
	SetIsNPC(bNPC);
	SetOwnerMesh(SkeletalMesh);
	SetAnimInstance(AnimationInstance);
	SetController(Controller);
	SetIsDead(bDead);
	bIsComponentSetup = true;
}

void UCharacterComponent::BeginPlay()
{
	Super::BeginPlay();
}

AController* UCharacterComponent::GetOwnerController()
{
	if(!CheckComponentIsSetup("Controller")){return nullptr;}
	return OwnerController;
}

APlayerController* UCharacterComponent::GetOwnerPlayerController()
{
	// bool bSetup = CheckComponentIsSetup("Player Controller"); 
	if(!CheckComponentIsSetup("Player Controller"))	{return nullptr;}
	return OwnerPlayerController;
}

USkeletalMeshComponent* UCharacterComponent::GetOwnerMesh()
{
	if(!CheckComponentIsSetup("Owner Mesh")){return nullptr;}
	return OwnerMesh;
}

UAnimInstance* UCharacterComponent::GetOwnerAnimInstance()
{
	if(!CheckComponentIsSetup("Animation Instance")){return nullptr;}
	return MainAnimInstance;
}

AAIController* UCharacterComponent::GetOwnerAIController()
{
	if(!CheckComponentIsSetup("AI Controller")){return nullptr;}
	return OwnerAIController;
}

void UCharacterComponent::SetController(AController* Controller)
{
	OwnerController = Controller;
	if(bIsNPC)
	{
		OwnerAIController = Cast<AAIController>(Controller);
	}
	else
	{
		OwnerPlayerController = Cast<APlayerController>(Controller);
	}
}

void UCharacterComponent::SetIsDead(const bool bDead)
{
	bIsDead = bDead;
	if(bIsDead)
	{
		OwnerDeath();
	}
}


void UCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCharacterComponent::OwnerDeath()
{
	OnDeathEvent();
	if(bDebuggingMode){UE_LOG(LogTemp, Warning, TEXT("Death"));}
}

void UCharacterComponent::SetInputModeGameAndUI(bool bGameAndUI, UWidget* InWidgetToFocus, bool bShowMouse)
{
	if(!CheckComponentIsSetup("Player Controller"))	{return;}
	if(bGameAndUI)
	{
		FInputModeGameAndUI InputModeData;
		OwnerPlayerController->SetInputMode(InputModeData);
		OwnerPlayerController->bShowMouseCursor = bShowMouse;
		if (InWidgetToFocus != nullptr)
		{
			InputModeData.SetWidgetToFocus(InWidgetToFocus->TakeWidget());
		}
	}
	else
	{
		FInputModeGameOnly InputModeData;
		OwnerPlayerController->SetInputMode(InputModeData);
		OwnerPlayerController->bShowMouseCursor = bShowMouse;
	}
}

bool UCharacterComponent::CheckComponentIsSetup(FString ComponentName)
{
	if(!bIsComponentSetup)
	{
		UE_LOG(LogTemp,Error, TEXT(" %s not set in %s of %s"), ToCStr(ComponentName), ToCStr(GetName()), ToCStr(GetOwner()->GetName()));
		SetActive(false);
		return false;
	}
	return bIsComponentSetup;
}

void UCharacterComponent::LogMissingPointer(FString PointerName) const
{
	UE_LOG(LogTemp,Error, TEXT(" %s not set in %s of %s"), ToCStr(PointerName), ToCStr(GetName()), ToCStr(GetOwner()->GetName()));
}

void UCharacterComponent::Server_PlayMontageAnimation_Implementation(UAnimMontage* MontageToPlay, float InPlayRate,
                                                                    EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages)
{
	LastAnimationDuration = MainAnimInstance->Montage_Play(MontageToPlay, InPlayRate, ReturnValueType, InTimeToStartMontageAt, bStopAllMontages);
	Multicast_PlayMontageAnimation(MontageToPlay, InPlayRate, ReturnValueType, InTimeToStartMontageAt, bStopAllMontages);
}

void UCharacterComponent::Multicast_PlayMontageAnimation_Implementation(UAnimMontage* MontageToPlay, float InPlayRate,
	EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages)
{
	MainAnimInstance->Montage_Play(MontageToPlay, InPlayRate, ReturnValueType, InTimeToStartMontageAt, bStopAllMontages);
}

void UCharacterComponent::Server_StopMontageAnimation_Implementation(float InBlendOutTime, const UAnimMontage* Montage)
{
	MainAnimInstance->Montage_Stop(InBlendOutTime, Montage);
	Multicast_StopMontageAnimation(InBlendOutTime, Montage);
}

void UCharacterComponent::Multicast_StopMontageAnimation_Implementation(float InBlendOutTime,
	const UAnimMontage* Montage)
{
	MainAnimInstance->Montage_Stop(InBlendOutTime, Montage);
}