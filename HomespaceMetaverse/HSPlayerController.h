#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "HSPlayerController.generated.h"


UCLASS()
class HOMESPACEMETAVERSE_API AHSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHSPlayerController(const FObjectInitializer& ObjectInitializer);
};