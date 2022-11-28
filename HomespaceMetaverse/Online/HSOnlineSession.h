#pragma once

#include "CoreMinimal.h"
#include "GameFramework/OnlineSession.h"
#include "HSOnlineSession.generated.h"

UCLASS()
class UHSOnlineSession : public UOnlineSession
{
	GENERATED_BODY()

public:
	virtual void HandleDisconnect(UWorld* World, UNetDriver* NetDriver) override;
	
};