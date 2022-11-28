#include "HSGameInstance.h"
#include "HSConstants.h"
#include "HSGameUserSettings.h"
#include "HSLocalPlayer.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/GameModeBase.h"
#include "SocketSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Misc/NetworkVersion.h"

#include "Online/HSOnlineSession.h"
#include "Misc/Paths.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#include "GenericPlatform/GenericApplication.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Engine/GameEngine.h"
#endif

UHSGameInstance::UHSGameInstance()
{

}

void UHSGameInstance::Init()
{
	Super::Init();

	InitSettings();
}

void UHSGameInstance::StartGameInstance()
{
	Super::StartGameInstance();
}

void UHSGameInstance::InitSettings()
{
	UHSGameUserSettings* Settings = UHSGameUserSettings::GetHSGameUserSettings();

	if (!IsValid(Settings))
		return;

	FGraphicsSettings Quality = Settings->GetQuality();
	if (Quality.bWasDefaultResolutionChanged)
	{
		Settings->SetFullscreenMode(Quality.bIsFullscreen ? EWindowMode::Fullscreen : EWindowMode::Windowed);
		Settings->SetScreenResolution(Quality.Resolution);
		Settings->ApplyResolutionSettings(true);
	}
	else
	{
		Quality.DefaultResolution = Settings->GetScreenResolution();
		Quality.Resolution = Quality.DefaultResolution;
		Settings->SetQuality(Quality);
	}
}

void UHSGameInstance::OnStart()
{
	Super::OnStart();
}

void UHSGameInstance::Shutdown()
{
	Super::Shutdown();
}

TSubclassOf<UOnlineSession> UHSGameInstance::GetOnlineSessionClass()
{
	return UHSOnlineSession::StaticClass();
}

UHSLocalPlayer* UHSGameInstance::GetLocalPlayer() const
{
	return Cast<UHSLocalPlayer>(GetLocalPlayerByIndex(0));
}
