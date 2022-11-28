#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "HSPlayerCharacter.generated.h"

UCLASS()
class HOMESPACEMETAVERSE_API AHSPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHSPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
};
