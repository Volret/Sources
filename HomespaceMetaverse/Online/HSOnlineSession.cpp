#include "HSOnlineSession.h"
#include "HSGameUserSettings.h"

void UHSOnlineSession::HandleDisconnect(UWorld* World, UNetDriver* NetDriver)
{
	if (UHSGameUserSettings::GetHSGameUserSettings()->bMatchEndNeedToShowInMainMenu)
	{
		// Prevent errors every tick
		GEngine->ShutdownWorldNetDriver(World);
	}
	else
	{
		Super::HandleDisconnect(World, NetDriver);
	}
}