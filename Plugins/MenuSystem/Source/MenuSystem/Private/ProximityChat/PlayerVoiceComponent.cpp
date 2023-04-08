// Fill out your copyright notice in the Description page of Project Settings.


#include "ProximityChat/PlayerVoiceComponent.h"
#include "Sound/SoundWaveProcedural.h"

void UPlayerVoiceComponent::PlayVoiceChat(const TArray<uint8>& VoiceData, int32 SampleRate)
{
	//- Create a new Sound Wave Procedural object	//
	USoundWaveProcedural* ProceduralSoundWave = NewObject<USoundWaveProcedural>();
	
	//- Set the procedural sound wave properties
	ProceduralSoundWave->SetSampleRate(SampleRate);
	ProceduralSoundWave->NumChannels = 1;
	ProceduralSoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	ProceduralSoundWave->SoundGroup = SOUNDGROUP_Voice;
	ProceduralSoundWave->bLooping = false;
	ProceduralSoundWave->bProcedural = true;

	//- Queue the voice chat data for playback	//
	ProceduralSoundWave->QueueAudio(VoiceData.GetData(), VoiceData.Num());

	//- Set the procedural sound wave as the audio source and play the audio	//
	SetSound(ProceduralSoundWave);
	Play();


}
