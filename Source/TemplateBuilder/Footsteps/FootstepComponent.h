// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "FootstepComponent.generated.h"



USTRUCT(BlueprintType)
struct FFootstepsVFXStruct
{
    GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Dirt;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* General01;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* General02;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Grass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Gravel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Ice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Leaves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Mud;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Sand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* SnowHeavy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* SnowLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Sparks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* SpecialAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* WaterHeavy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* WaterLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Carpet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Concrete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Metal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Plastic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
    class UNiagaraSystem* Wood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	class UNiagaraSystem* Flesh;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATEBUILDER_API UFootstepComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFootstepComponent();

	//Components
	// UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	// class UAudioComponent* FootstepAC;
	// UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	// class UAudioComponent* JumpStartAC;
	// UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	// class UAudioComponent* JumpEndAC;


	UFUNCTION(BlueprintCallable, Category = "Footsteps")
	void PlayRightFootstepSoundEvent(ACharacter* Player);
	UFUNCTION(BlueprintCallable, Category = "Footsteps")
	void PlayRightFootstepVFXEvent(ACharacter* Player, int FloorType, float Speed);
	UFUNCTION(BlueprintCallable, Category = "Footsteps")
	void PlayLeftFootstepSoundEvent(ACharacter* Player);
	UFUNCTION(BlueprintCallable, Category = "Footsteps")
	void PlayLeftFootstepVFXEvent(ACharacter* Player, int FloorType, float Speed);

	UFUNCTION(BlueprintCallable, Category = "Footsteps")
	void PlayJumpStartSoundEvent(ACharacter* Player);
	UFUNCTION(BlueprintCallable, Category = "Footsteps")
	void PlayJumpEndSoundEvent(ACharacter* Player);



protected:
	void FootstepVFXSelector(const int in_FloorType,const float in_Speed, FVector& out_Scale, UNiagaraSystem*& out_FootstepParticle);

	//Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* FootstepSFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* JumpStartSFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* JumpEndSFX;


	//Footstep Vars//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep Variables")
	bool bFootstepsPlay = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep Variables")
	bool bRattlePlay = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep Variables")
	bool bVoiceJumpPlay = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep Variables")
	float FootstepsVolume = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep Variables")
	float RattleVolume = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep Variables")
	float VoiceJumpVolume = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep Variables")
	bool bIsMale = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep Variables")
	bool bIsPawnCharacter = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep Variables")
	bool bPlayVFX = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep Variables")
	float VFXSize = 1.0f;


	//VFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FFootstepsVFXStruct VFX;


	//Put Floor Materials here //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<class UPhysicalMaterial*> Surfaces;

private:
	//Components
	class UAudioComponent* FootstepAC;
	class UAudioComponent* JumpStartAC;
	class UAudioComponent* JumpEndAC;

	bool ValidateAssets(ACharacter* Player);
};
