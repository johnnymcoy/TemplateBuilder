// Fill out your copyright notice in the Description page of Project Settings.


#include "FootstepComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"


#include "DrawDebugHelpers.h"
#include "TemplateBuilder/AI/AIControllerBase.h"


// Sets default values for this component's properties
UFootstepComponent::UFootstepComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetComponentTickEnabled(false);
}

void UFootstepComponent::PlayRightFootstepSoundEvent(ACharacter* Player) 
{	
	if(ValidateAssets(Player))
	{
		FootstepAC = UGameplayStatics::SpawnSoundAtLocation(this, FootstepSFX, Player->GetMesh()->GetSocketLocation(TEXT("RightFootSocket")));
		FootstepAC->Play();
		FootstepAC->SetBoolParameter(TEXT("step_play"), bFootstepsPlay);
		FootstepAC->SetBoolParameter(TEXT("rattle_play"), bRattlePlay);
		float Speed = UKismetMathLibrary::MapRangeClamped(GetOwner()->GetVelocity().Size(), 200, 600, 0.55, 1.0);
		FootstepAC->SetFloatParameter(TEXT("step_volume"), (FootstepsVolume * Speed));
		FootstepAC->SetFloatParameter(TEXT("rattle_volume"), (RattleVolume * Speed));
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(GetOwner());
		TraceParams.bReturnPhysicalMaterial = true;
		FHitResult Hit;
		FVector TraceDown;
		TraceDown.Z = -150.f;
		GetWorld()->LineTraceSingleByChannel(Hit, GetOwner()->GetActorLocation(), ((GetOwner()->GetActorLocation()) + (TraceDown)), ECC_Visibility, TraceParams);
		int FloorIndex = (Surfaces.Find(Hit.PhysMaterial.Get()));
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		FootstepAC->SetIntParameter(TEXT("floor"), (FloorIndex + 1.f)); // +1 to counter the fact that the surfaces start at -1
		FootstepAC->SetBoolParameter(TEXT("IsMale"), bIsMale);

		PlayRightFootstepVFXEvent(Player, FloorIndex, Speed);
	}
}

void UFootstepComponent::PlayRightFootstepVFXEvent(ACharacter* Player, int FloorType, float Speed) 
{
	FVector Scale;
	UNiagaraSystem* FootstepParticle = nullptr; //VFX.Dirt
	FootstepVFXSelector(FloorType, Speed, Scale, FootstepParticle);
	FRotator Rotation;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FootstepParticle, Player->GetMesh()->GetSocketLocation(TEXT("RightFootSocket")), Rotation, Scale);
}

void UFootstepComponent::PlayLeftFootstepSoundEvent(ACharacter* Player) 
{
	if(ValidateAssets(Player))
	{
		FootstepAC = UGameplayStatics::SpawnSoundAtLocation(this, FootstepSFX, Player->GetMesh()->GetSocketLocation(TEXT("LeftFootSocket")));
		FootstepAC->Play();
		FootstepAC->SetBoolParameter(TEXT("step_play"), bFootstepsPlay);
		FootstepAC->SetBoolParameter(TEXT("rattle_play"), bRattlePlay);
		float Speed = UKismetMathLibrary::MapRangeClamped(GetOwner()->GetVelocity().Size(), 200, 600, 0.55, 1.0);
		FootstepAC->SetFloatParameter(TEXT("step_volume"), (FootstepsVolume * Speed));
		FootstepAC->SetFloatParameter(TEXT("rattle_volume"), (RattleVolume * Speed));
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(GetOwner());
		TraceParams.bReturnPhysicalMaterial = true;
		FHitResult Hit;
		FVector TraceDown;
		TraceDown.Z = -150.f;
		GetWorld()->LineTraceSingleByChannel(Hit, GetOwner()->GetActorLocation(), ((GetOwner()->GetActorLocation()) + (TraceDown)), ECC_Visibility, TraceParams);
		int FloorIndex = (Surfaces.Find(Hit.PhysMaterial.Get()));
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		FootstepAC->SetIntParameter(TEXT("floor"), (FloorIndex + 1.f)); // +1 to counter the fact that the surfaces start at -1
		FootstepAC->SetBoolParameter(TEXT("IsMale"), bIsMale);

		PlayLeftFootstepVFXEvent(Player, FloorIndex, Speed);
	}
}

void UFootstepComponent::PlayLeftFootstepVFXEvent(ACharacter* Player, int FloorType, float Speed) 
{
	FVector Scale;
	UNiagaraSystem* FootstepParticle = nullptr; //VFX.Dirt
	FootstepVFXSelector(FloorType, Speed, Scale, FootstepParticle);
	FRotator Rotation;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FootstepParticle, Player->GetMesh()->GetSocketLocation(TEXT("LeftFootSocket")), Rotation, Scale);

}

void UFootstepComponent::FootstepVFXSelector(const int in_FloorType,const float in_Speed, FVector& out_Scale, UNiagaraSystem*& out_FootstepParticle) 
{

	out_Scale.X = (in_Speed * VFXSize);
	out_Scale.Y = (in_Speed * VFXSize);
	out_Scale.Z = (in_Speed * VFXSize);

	switch (in_FloorType)
	{
	case -1: //Default Ground
		out_FootstepParticle = VFX.General01;
		break;
	case 0: //Dirt
		out_FootstepParticle = VFX.Dirt;
		break;
	case 1: //General 01
		out_FootstepParticle = VFX.General01;
		break;
	case 2: //General 02
		out_FootstepParticle = VFX.General02;
		break;	
	case 3: //Grass
		out_FootstepParticle = VFX.Grass;
		break;		
	case 4: //Gravel
		out_FootstepParticle = VFX.Gravel;
		break;	
	case 5: //Ice
		out_FootstepParticle = VFX.Ice;
		break;		
	case 6: //Leaves
		out_FootstepParticle = VFX.Leaves;
		break;
	case 7: //Mud
		out_FootstepParticle = VFX.Mud;
		break;		
	case 8: //Sand
		out_FootstepParticle = VFX.Sand;
		break;		
	case 9: //SnowHeavy
		out_FootstepParticle = VFX.SnowHeavy;
		break;		
	case 10: //SnowLight
		out_FootstepParticle = VFX.SnowLight;
		break;		
	case 11: //Sparks
		out_FootstepParticle = VFX.Sparks;
		break;		
	case 12: //SpecialAbility
		out_FootstepParticle = VFX.SpecialAbility;
		break;		
	case 13: //WaterHeavy
		out_FootstepParticle = VFX.WaterHeavy;
		break;	
	case 14: //WaterLight
		out_FootstepParticle = VFX.WaterLight;
		break;	
	case 15: //Carpet
		out_FootstepParticle = VFX.Carpet;
		break;	
	case 16: //Concrete
		out_FootstepParticle = VFX.Concrete;
		break;
	case 17: //Metal
		out_FootstepParticle = VFX.Metal;
		break;
	case 18: //Plastic
		out_FootstepParticle = VFX.Plastic;
		break;
	case 19: //Wood
		out_FootstepParticle = VFX.Wood;
		break;
	case 20: //Flesh todo Add flesh FootstepFX
		out_FootstepParticle = VFX.Wood;
		break;
	default: out_FootstepParticle = VFX.General01;
		break;
	}

}

void UFootstepComponent::PlayJumpStartSoundEvent(ACharacter* Player) 
{
	if(ValidateAssets(Player))
	{
		JumpStartAC = UGameplayStatics::SpawnSoundAtLocation(this, JumpStartSFX, Player->GetMesh()->GetSocketLocation(TEXT("LeftFootSocket")));
		JumpStartAC->Play();
		JumpStartAC->SetBoolParameter(TEXT("step_play"), bFootstepsPlay);
		JumpStartAC->SetBoolParameter(TEXT("rattle_play"), bRattlePlay);
		JumpStartAC->SetFloatParameter(TEXT("step_volume"), FootstepsVolume);
		JumpStartAC->SetFloatParameter(TEXT("rattle_volume"), RattleVolume);
		JumpStartAC->SetFloatParameter(TEXT("voice_volume"), VoiceJumpVolume);
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(GetOwner());
		TraceParams.bReturnPhysicalMaterial = true;
		FHitResult Hit;
		FVector TraceDown;
		TraceDown.Z = -150.f;
		GetWorld()->LineTraceSingleByChannel(Hit, GetOwner()->GetActorLocation(), ((GetOwner()->GetActorLocation()) + (TraceDown)), ECC_Visibility, TraceParams);
		int FloorIndex = (Surfaces.Find(Hit.PhysMaterial.Get()));
		JumpStartAC->SetIntParameter(TEXT("floor"), (FloorIndex + 1.f)); // +1 to counter the fact that the surfaces start at -1
		JumpStartAC->SetBoolParameter(TEXT("IsMale"), bIsMale);
	}
}

void UFootstepComponent::PlayJumpEndSoundEvent(ACharacter* Player) 
{
	if(ValidateAssets(Player))
	{
		JumpEndAC = UGameplayStatics::SpawnSoundAtLocation(this, JumpStartSFX, Player->GetMesh()->GetSocketLocation(TEXT("LeftFootSocket")));
		JumpEndAC->Play();
		JumpEndAC->SetBoolParameter(TEXT("step_play"), bFootstepsPlay);
		JumpEndAC->SetBoolParameter(TEXT("rattle_play"), bRattlePlay);
		JumpEndAC->SetFloatParameter(TEXT("step_volume"), FootstepsVolume);
		JumpEndAC->SetFloatParameter(TEXT("rattle_volume"), RattleVolume);
		JumpEndAC->SetFloatParameter(TEXT("voice_volume"), VoiceJumpVolume);
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(GetOwner());
		TraceParams.bReturnPhysicalMaterial = true;
		FHitResult Hit;
		FVector TraceDown;
		TraceDown.Z = -150.f;
		GetWorld()->LineTraceSingleByChannel(Hit, GetOwner()->GetActorLocation(), ((GetOwner()->GetActorLocation()) + (TraceDown)), ECC_Visibility, TraceParams);
		int FloorIndex = (Surfaces.Find(Hit.PhysMaterial.Get()));
		JumpEndAC->SetIntParameter(TEXT("floor"), (FloorIndex + 1.f)); // +1 to counter the fact that the surfaces start at -1
		JumpEndAC->SetBoolParameter(TEXT("IsMale"), bIsMale);
	}
}


bool UFootstepComponent::ValidateAssets(ACharacter* Player) 
{
	if(Player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is not attached to Event"));
		return false;
	}
	if(FootstepSFX == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Please add Footstep SFX"));
		return false;
	}
	if(JumpStartSFX == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Please add JumpStart SFX"));
		return false;
	}
	if(JumpEndSFX == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Please add JumpEnd SFX"));
		return false;
	}
	if(Surfaces.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Surface Materials have been set up!"));
		return true;
	}
	return true;
}


