// Fill out your copyright notice in the Description page of Project Settings.


#include "ProximityChat/ProximityChatBPLib.h"

float UProximityChatBPLib::CalculateVoiceAttenuation(const FVector& SpeakerLocation, const FVector& ListenerLocation,
	float MaxDistance, float VolumeAtMaxDistance)
{
	//- Calculate the distance between the speaker and the listener	//
	float Distance = FVector::Distance(SpeakerLocation, ListenerLocation);
	//- Check if the listener is farther than the maximum distance	//
	if(Distance > MaxDistance)
	{
		//- The listener is too far away, no volume	//
		return 0.0f;
	}
	//- Calculate the attenuation factor based on the distance and the maximum distance	//
	const float AttenuationFactor = 1.0f - (Distance / MaxDistance);
	
	//- Calculate the volume of the voice chat based on the attenuation factor and the volume at the maximum distance	//
	const float VoiceVolume = FMath::Lerp(VolumeAtMaxDistance, 1.0f, AttenuationFactor);
	return VoiceVolume;


}
