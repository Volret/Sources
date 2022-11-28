#include "AudioHelpers.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UAudioComponent* AudioHelpers::Spawn2D(const UObject* WorldContextObject,
                                       USoundCue* Cue,
                                       float VolumeMultiplier)
{
	if (IsValid(Cue))
	{
		return UGameplayStatics::SpawnSound2D(WorldContextObject, Cue, VolumeMultiplier);
	}

	return nullptr;
}

UAudioComponent* AudioHelpers::Spawn3D(const UObject* WorldContextObject, 
									   USoundCue* Cue, 
									   const FVector& Location, 
									   float VolumeMultiplier)
{
	if (IsValid(Cue))
	{
		return UGameplayStatics::SpawnSoundAtLocation(WorldContextObject, Cue, Location, FRotator::ZeroRotator, VolumeMultiplier);
	}

	return nullptr;
}

void AudioHelpers::StartCrossfade(UAudioComponent* AudioOut,
								  UAudioComponent* AudioIn, 
								  float Duration)
{
	if (IsValid(AudioOut))
	{
		AudioOut->FadeOut(Duration, 0.f);
	}

	if (IsValid(AudioIn))
	{
		AudioIn->FadeIn(Duration, 1.f);
	}
}

void AudioHelpers::StartCrossfade(UAudioComponent* AudioOut,
								  UAudioComponent* AudioIn,
								  float FadeOutDuration, 
								  float FadeInDuration,
								  float FadeInPredelay)
{
	if (IsValid(AudioOut))
	{
		AudioOut->FadeOut(FadeOutDuration, 0.f);
	}

	if (IsValid(AudioIn))
	{
		AudioIn->FadeIn(FadeInDuration, 1.f, FadeInPredelay);
	}
}
