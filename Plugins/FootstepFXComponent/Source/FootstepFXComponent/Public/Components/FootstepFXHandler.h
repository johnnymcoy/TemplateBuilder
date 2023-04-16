// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterComponent.h"
#include "FootstepFXHandler.generated.h"

USTRUCT(BlueprintType)
struct FFootstepsVFX
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
class FOOTSTEPFXCOMPONENT_API UFootstepFXHandler : public UCharacterComponent
{
	GENERATED_BODY()

public:	
	UFootstepFXHandler();

	UFUNCTION(BlueprintCallable, Category = "Footsteps")
	void PlayFootstepSoundEvent(bool bRightFoot);
	UFUNCTION(BlueprintCallable, Category = "Footsteps")
	void PlayFootstepVFXEvent(int FloorType, float Speed, bool bRightFoot);

	UFUNCTION(BlueprintCallable, Category = "Footsteps")
	void PlayJumpSoundEvent(bool bRightFoot, bool bJumpStart);


protected:
	virtual void BeginPlay() override;
	void FootstepVFXSelector(const int in_FloorType,const float in_Speed, FVector& out_Scale, UNiagaraSystem*& out_FootstepParticle);

	//Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	class USoundBase* FootstepSFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	class USoundBase* JumpStartSFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	class USoundBase* JumpEndSFX;


	//-		Footstep Variables	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	bool bFootstepsPlay = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	bool bRattlePlay = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	bool bVoiceJumpPlay = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	float FootstepsVolume = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	float RattleVolume = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	float VoiceJumpVolume = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	bool bIsMale = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	bool bIsPawnCharacter = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	bool bPlayVFX = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	float VFXSize = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	FString RightFootSocketName = "RightFootSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	FString LeftFootSocketName = "LeftFootSocket";

	//VFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	FFootstepsVFX VFX;


	//-		Put Floor Materials here	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footsteps")
	TArray<class UPhysicalMaterial*> Surfaces;


private:
	//-		TraceParams for finding Surface below character		//
	FCollisionQueryParams TraceParams;
	FVector TraceDown;

	
	//-		Components		//
	UPROPERTY()
	class UAudioComponent* FootstepAC;
	UPROPERTY()
	class UAudioComponent* JumpStartAC;
	UPROPERTY()
	class UAudioComponent* JumpEndAC;
			
};

