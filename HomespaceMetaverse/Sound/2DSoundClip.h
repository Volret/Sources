#pragma once

#include "CoreMinimal.h"
#include "2DSoundClip.generated.h"

USTRUCT(BlueprintType)
struct HOMESPACEMETAVERSE_API F2DSoundClip
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VolumeMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundCue* Cue;

	void Play(UObject* WorldContextObject) const;
	
	void PlayAt(UObject* WorldContextObject, FVector Location) const;

	UAudioComponent* Spawn(UObject* WorldContextObject) const;

	UAudioComponent* SpawnAt(UObject* WorldContextObject, FVector Location) const;
};

