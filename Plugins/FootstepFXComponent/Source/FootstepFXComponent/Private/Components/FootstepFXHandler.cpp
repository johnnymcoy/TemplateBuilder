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
	ConstructorHelpers::FObjectFinder<USoundBase> const FootstepsSoundCue(TEXT("/FootstepFXComponent/Sounds/Sound_Cues/Footsteps"));
	if (!ensure(FootstepsSoundCue.Object != nullptr)) return;
	FootstepSFX = FootstepsSoundCue.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> const JumpStartSoundCue(TEXT("/FootstepFXComponent/Sounds/Sound_Cues/Jump_Start"));
	if (!ensure(JumpStartSoundCue.Object != nullptr)) return;
	JumpStartSFX = JumpStartSoundCue.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> const JumpEndSoundCue(TEXT("/FootstepFXComponent/Sounds/Sound_Cues/Jump_End"));
	if (!ensure(JumpEndSoundCue.Object != nullptr)) return;
	JumpEndSFX = JumpEndSoundCue.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const DirtNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_Dirt_Surface"));
	if (!ensure(DirtNiagaraSystem.Object != nullptr)) return;
	VFX.Dirt = DirtNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const General1NiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_General1_Surface"));
	if (!ensure(General1NiagaraSystem.Object != nullptr)) return;
	VFX.General01 = General1NiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const General2NiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_General2_Surface"));
	if (!ensure(General2NiagaraSystem.Object != nullptr)) return;
	VFX.General02 = General2NiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const GrassNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_Grass_Surface"));
	if (!ensure(GrassNiagaraSystem.Object != nullptr)) return;
	VFX.Grass = GrassNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const GravelNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_Gravel_Surface"));
	if (!ensure(GravelNiagaraSystem.Object != nullptr)) return;
	VFX.Gravel = GravelNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const IceNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_Ice_Surface"));
	if (!ensure(IceNiagaraSystem.Object != nullptr)) return;
	VFX.Ice = IceNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const LeavesNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_Leaves_Surface"));
	if (!ensure(LeavesNiagaraSystem.Object != nullptr)) return;
	VFX.Leaves = LeavesNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const MudNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_Mud_Surface"));
	if (!ensure(MudNiagaraSystem.Object != nullptr)) return;
	VFX.Mud = MudNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const SandNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_Sand_Surface"));
	if (!ensure(SandNiagaraSystem.Object != nullptr)) return;
	VFX.Sand = SandNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const SnowHeavyNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_SnowHeavy_Surface"));
	if (!ensure(SnowHeavyNiagaraSystem.Object != nullptr)) return;
	VFX.SnowHeavy = SnowHeavyNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const SnowLightNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_SnowLight_Surface"));
	if (!ensure(SnowLightNiagaraSystem.Object != nullptr)) return;
	VFX.SnowLight = SnowLightNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const SparksNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_Sparks_Surface"));
	if (!ensure(SparksNiagaraSystem.Object != nullptr)) return;
	VFX.Sparks = SparksNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const SpecialAbilityNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_SpecialAbility_Surface"));
	if (!ensure(SpecialAbilityNiagaraSystem.Object != nullptr)) return;
	VFX.SpecialAbility = SpecialAbilityNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const WaterHeavyNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_WaterHeavy_Surface"));
	if (!ensure(WaterHeavyNiagaraSystem.Object != nullptr)) return;
	VFX.WaterHeavy = WaterHeavyNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> const WaterLightNiagaraSystem(TEXT("/FootstepFXComponent/FootstepParticles/ParticleSystems/PSN_WaterLight_Surface"));
	if (!ensure(WaterLightNiagaraSystem.Object != nullptr)) return;
	VFX.WaterLight = WaterLightNiagaraSystem.Object;
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const DirtPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/01_Dirt_Physic_Mat"));
	if (!ensure(DirtPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(DirtPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const General1PhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/02_General01_Physic_Mat"));
	if (!ensure(General1PhysicalMat.Object != nullptr)) return;
	Surfaces.Add(General1PhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const General2PhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/03_General02_Physic_Mat"));
	if (!ensure(General2PhysicalMat.Object != nullptr)) return;
	Surfaces.Add(General2PhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const GrassPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/04_Grass_Physic_Mat"));
	if (!ensure(GrassPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(GrassPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const GravelPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/05_Gravel_Physic_Mat"));
	if (!ensure(GravelPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(GravelPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const IcePhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/06_Ice_Physic_Mat"));
	if (!ensure(IcePhysicalMat.Object != nullptr)) return;
	Surfaces.Add(IcePhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const LeavesPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/07_Leaves_Physic_Mat"));
	if (!ensure(LeavesPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(LeavesPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const MudPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/08_Mud_Physic_Mat"));
	if (!ensure(MudPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(MudPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const SandPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/09_Sand_Physic_Mat"));
	if (!ensure(SandPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(SandPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const SnowHeavyPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/10_SnowHeavy_Physic_Mat"));
	if (!ensure(SnowHeavyPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(SnowHeavyPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const SnowLightPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/11_SnowLight_Physic_Mat"));
	if (!ensure(SnowLightPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(SnowLightPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const SparksPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/12_Sparks_Physic_Mat"));
	if (!ensure(SparksPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(SparksPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const SpecialAbilityPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/13_SpecialAbility_Physic_Mat"));
	if (!ensure(SpecialAbilityPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(SpecialAbilityPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const WaterHeavyPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/14_WaterHeavy_Physic_Mat"));
	if (!ensure(WaterHeavyPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(WaterHeavyPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const WaterLightPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/15_WaterLight_Physic_Mat"));
	if (!ensure(WaterLightPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(WaterLightPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const CarpetPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/16_Carpet_Physic_Mat"));
	if (!ensure(CarpetPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(CarpetPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const ConcretePhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/17_Concrete_Physic_Mat"));
	if (!ensure(ConcretePhysicalMat.Object != nullptr)) return;
	Surfaces.Add(ConcretePhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const MetalPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/18_Metal_Physic_Mat"));
	if (!ensure(MetalPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(MetalPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const PlasticPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/19_Plastic_Physic_Mat"));
	if (!ensure(PlasticPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(PlasticPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const WoodPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/20_Wood_Physic_Mat"));
	if (!ensure(WoodPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(WoodPhysicalMat.Object);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> const FleshPhysicalMat(TEXT("/FootstepFXComponent/PhysicalMaterials/21_Flesh_Physic_Mat"));
	if (!ensure(FleshPhysicalMat.Object != nullptr)) return;
	Surfaces.Add(FleshPhysicalMat.Object);
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
	FootstepAC->SetBoolParameter(TEXT("step_play"), bFootstepsPlay);
	FootstepAC->SetBoolParameter(TEXT("rattle_play"), bRattlePlay);
	const float Speed = UKismetMathLibrary::MapRangeClamped(GetOwner()->GetVelocity().Size(), 200.0f, 600.0f, 0.55f, 1.0f);
	FootstepAC->SetFloatParameter(TEXT("step_volume"), (FootstepsVolume * Speed));
	FootstepAC->SetFloatParameter(TEXT("rattle_volume"), (RattleVolume * Speed));
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, GetOwner()->GetActorLocation(), (GetOwner()->GetActorLocation() + TraceDown), ECC_Visibility, TraceParams);
	const int FloorIndex = (Surfaces.Find(Hit.PhysMaterial.Get()));
	if(bDebuggingMode){DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Green, true, 3.0f);UE_LOG(LogTemp,Warning,TEXT("Floor Index: %i"), FloorIndex);}
	//-		+1 to counter the face that the surfaces start at -1	//
	FootstepAC->SetIntParameter(TEXT("floor"), (FloorIndex + 1.0f));
	FootstepAC->SetBoolParameter(TEXT("IsMale"), bIsMale);
	FootstepAC->Play();
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



