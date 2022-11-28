#include "HSPlayerController.h"

#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GameFramework/PlayerState.h"
#include "Hash/CityHash.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AHSPlayerController::AHSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}