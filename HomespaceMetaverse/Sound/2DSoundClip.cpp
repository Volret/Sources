#include "2DSoundClip.h"
#include "Kismet/GameplayStatics.h"
#include "AudioHelpers.h"

void F2DSoundClip::Play(UObject* WorldContextObject) const
{
	AudioHelpers::Spawn2D(WorldContextObject, Cue, VolumeMultiplier);
}

void F2DSoundClip::PlayAt(UObject* WorldContextObject, FVector Location) const
{
	AudioHelpers::Spawn3D(WorldContextObject, Cue, Location, VolumeMultiplier);
}

UAudioComponent* F2DSoundClip::Spawn(UObject* WorldContextObject) const
{
	return AudioHelpers::Spawn2D(WorldContextObject, Cue, VolumeMultiplier);
}

UAudioComponent* F2DSoundClip::SpawnAt(UObject* WorldContextObject, FVector Location) const
{
	return AudioHelpers::Spawn3D(WorldContextObject, Cue, Location, VolumeMultiplier);
}
