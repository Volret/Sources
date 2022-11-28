#pragma once

#include "Online.h"

class FDefaultOnlineSessionSettings : public FOnlineSessionSettings
{
public:

	FDefaultOnlineSessionSettings(bool bIsLAN = false, bool bIsPresence = false, int32 MaxNumPlayers = 4);
	virtual ~FDefaultOnlineSessionSettings() {}
};

class FDefaultOnlineSearchSettings : public FOnlineSessionSearch
{
public:
	FDefaultOnlineSearchSettings(bool bSearchingLAN = false, bool bSearchingPresence = false);

	virtual ~FDefaultOnlineSearchSettings() {}
};

class FDefaultOnlineSearchSettingsEmptyDedicated : public FDefaultOnlineSearchSettings
{
public:
	FDefaultOnlineSearchSettingsEmptyDedicated(bool bSearchingLAN = false, bool bSearchingPresence = false);

	virtual ~FDefaultOnlineSearchSettingsEmptyDedicated() {}
};
