// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FootstepFXHandler.h"

#include "DrawDebugHelpers.h"
#include "Components/AudioComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "NiagaraFunctionLibrary.h"



UFootstepFXHandler::UFootstepFXHandler()
{
	PrimaryComponentTick.bCanEverTick = false;
	// SetComponentTickEnabled(false);
}

void UFootstepFXHandler::BeginPlay()
{
	Super::BeginPlay();
	TraceParams.AddIgnoredActor(GetOwner());
	TraceParams.bReturnPhysicalMaterial = true;
	TraceDown.Z = -150.f;
}


void UFootstepFXHandler::PlayFootstepSoundEvent(bool bRightFoot)
{
	if(GetOwnerMesh() == nullptr){LogMissingPointer("Owner Mesh");return;}
	FName SocketName = FName(*LeftFootSocketName);
	if(bRightFoot){SocketName = FName(*RightFootSocketName);}
	FootstepAC = UGameplayStatics::SpawnSoundAtLocation(this, FootstepSFX, GetOwnerMesh()->GetSocketLocation(SocketName));
	FootstepAC->Play();
	FootstepAC->SetBoolParameter(TEXT("step_play"), bFootstepsPlay);
	FootstepAC->SetBoolParameter(TEXT("rattle_play"), bRattlePlay);
	float Speed = UKismetMathLibrary::MapRangeClamped(GetOwner()->GetVelocity().Size(), 200.0f, 600.0f, 0.55f, 1.0f);
	FootstepAC->SetFloatParameter(TEXT("step_volume"), (FootstepsVolume * Speed));
	FootstepAC->SetFloatParameter(TEXT("rattle_volume"), (RattleVolume * Speed));
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, GetOwner()->GetActorLocation(), (GetOwner()->GetActorLocation() + TraceDown), ECC_Visibility, TraceParams);
	int FloorIndex = (Surfaces.Find(Hit.PhysMaterial.Get()));
	if(bDebuggingMode){DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Green, true);}
	//-		+1 to counter the face that the surfaces start at -1	//
	FootstepAC->SetIntParameter(TEXT("floor"), (FloorIndex + 1.0f));
	FootstepAC->SetBoolParameter(TEXT("IsMale"), bIsMale);
	PlayFootstepVFXEvent(FloorIndex, Speed, bRightFoot);
}

void UFootstepFXHandler::PlayFootstepVFXEvent(int FloorType, float Speed, bool bRightFoot)
{
	FVector Scale;
	UNiagaraSystem* FootstepParticle = nullptr; //VFX.Dirt
	FootstepVFXSelector(FloorType, Speed, Scale, FootstepParticle);
	FRotator Rotation;
	FName SocketName = FName(*LeftFootSocketName);
	if(bRightFoot){SocketName = FName(*RightFootSocketName);}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FootstepParticle, GetOwnerMesh()->GetSocketLocation(SocketName), Rotation, Scale);
}

void UFootstepFXHandler::FootstepVFXSelector(const int in_FloorType, const float in_Speed, FVector& out_Scale,
	UNiagaraSystem*& out_FootstepParticle)
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

void UFootstepFXHandler::PlayJumpSoundEvent(bool bRightFoot, bool bJumpStart)
{
	if(GetOwnerMesh() == nullptr){LogMissingPointer("Owner Mesh");return;}
	FName SocketName = FName(*LeftFootSocketName);
	if(bRightFoot){SocketName = FName(*RightFootSocketName);}
	USoundBase* JumpSFX = JumpEndSFX;
	UAudioComponent* JumpAC = JumpEndAC;
	if(bJumpStart)
	{
		JumpSFX = JumpStartSFX;
		JumpAC = JumpStartAC;
	}
	JumpAC = UGameplayStatics::SpawnSoundAtLocation(this, JumpSFX, GetOwnerMesh()->GetSocketLocation(SocketName));
	JumpAC->Play();
	JumpAC->SetBoolParameter(TEXT("step_play"), bFootstepsPlay);
	JumpAC->SetBoolParameter(TEXT("rattle_play"), bRattlePlay);
	JumpAC->SetFloatParameter(TEXT("step_volume"), FootstepsVolume);
	JumpAC->SetFloatParameter(TEXT("rattle_volume"), RattleVolume);
	JumpAC->SetFloatParameter(TEXT("voice_volume"), VoiceJumpVolume);
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, GetOwner()->GetActorLocation(), ((GetOwner()->GetActorLocation()) + (TraceDown)), ECC_Visibility, TraceParams);
	int FloorIndex = (Surfaces.Find(Hit.PhysMaterial.Get()));
	//- +1 to counter the fact that the surfaces start at -1
	JumpAC->SetIntParameter(TEXT("floor"), (FloorIndex + 1.f));
	JumpAC->SetBoolParameter(TEXT("IsMale"), bIsMale);
}



