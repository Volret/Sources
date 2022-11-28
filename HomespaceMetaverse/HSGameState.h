#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HSConstants.h"
#include "HSGameState.generated.h"


UCLASS()
class HOMESPACEMETAVERSE_API AHSGameState : public AGameState
{
	GENERATED_BODY()

public:
	AHSGameState(const FObjectInitializer& ObjectInitializer);
};
