#include "HSLocalPlayer.h"

#include "HSGameState.h"
#include "HSGameUserSettings.h"
#include "HSHelpers.h"
#include "HSPlayerController.h"

FString UHSLocalPlayer::GetNickname() const
{
	if (const auto UserSettings = UHSGameUserSettings::GetHSGameUserSettings())
	{
		return UserSettings->GetPlayerName();
	}

	return Super::GetNickname();
}