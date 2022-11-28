#pragma once

class UAudioComponent;
class USoundCue;

/// Helper class with methods which operate with game engine audio.
class AudioHelpers
{
public:
	/// <summary>
	/// Spawns AudioComponent which sounds in 2D space. 
	/// </summary>
	/// <param name="WorldContextObject">Object to get corresponding UWorld. </param>
	/// <param name="Cue">SoundCue asset to play. </param>
	/// <param name="VolumeMultiplier">Volume multiplier where 0.f is silence and 1.f means no gain changes. </param>
	/// <returns>Spawned audio component. </returns>
	static UAudioComponent* Spawn2D(const UObject* WorldContextObject, 
									USoundCue* Cue, 
									float VolumeMultiplier = 1.f);

	/// <summary>
	/// Spawns AudioComponent which sounds in 3D space. 
	/// </summary>
	/// <param name="WorldContextObject">Object to get corresponding UWorld. </param>
	/// <param name="Cue">SoundCue asset to play. </param>
	/// <param name="Location">Where sound should be play at. </param>
	/// <param name="VolumeMultiplier">Volume multiplier where 0.f is silence and 1.f means no gain changes. </param>
	/// <returns>Spawned audio component. </returns>
	static UAudioComponent* Spawn3D(const UObject* WorldContextObject, 
									USoundCue* Cue, 
									const FVector& Location, 
									float VolumeMultiplier = 1.f);

	/// <summary>
	/// Cross fades two started audios. 
	/// </summary>
	/// <param name="AudioOut">May be invalid. Audio component to fade out from. </param>
	/// <param name="AudioIn">May be invalid. Audio component to fade in to. </param>
	/// <param name="Duration">Cross fade duration of both. </param>
	static void StartCrossfade(UAudioComponent* AudioOut, 
							   UAudioComponent* AudioIn, 
							   float Duration);;

	/// <summary>
	/// Cross fades two started audios. 
	/// </summary>
	/// <param name="AudioOut">May be invalid. Audio component to fade out from. </param>
	/// <param name="AudioIn">May be invalid. Audio component to fade in to. </param>
	/// <param name="FadeOutDuration">Fade out duration. </param>
	/// <param name="FadeInDuration">Fade in duration. </param>
	/// <param name="FadeInPredelay">Fade in pre delay. </param>
	static void StartCrossfade(UAudioComponent* AudioOut,
							   UAudioComponent* AudioIn,
							   float FadeOutDuration, 
							   float FadeInDuration,
							   float FadeInPredelay);
};