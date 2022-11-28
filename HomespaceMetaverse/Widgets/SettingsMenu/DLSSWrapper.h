// Utulities for working with DLSS in the context of UI settings

#pragma once

#include "SettingsConsts.h"
#include "Blueprint/UserWidget.h"
#include "DLSSLibrary.h"

// DLSS Modes used in UI. Note - the order differs from UDLSSMode
enum DLSSModeUI : uint8
{
	DLSSUI_Off = 0,
	DLSSUI_Auto = 1,
	DLSSUI_UltraPerformance = 2,
	DLSSUI_Performance = 3,
	DLSSUI_Balanced = 4,
	DLSSUI_Quality = 5,
	DLSSUI_UltraQuality = 6,
	DLSSUI_N = 7
};

// Wrapper functions for DLSS
class DLSSWrapper
{
public:
	static UDLSSMode DLSSUI2UDLSS(DLSSModeUI modeui);
	static DLSSModeUI UDLSS2DLSSUI(UDLSSMode mode);

	static TArray<FText> GetAvailableDlssModes();

	static int ModeToListIndex(UDLSSMode mode);	

	static bool IsDLSSSupported();
	static bool ModeIsSupported(UDLSSMode mode);

	struct Settings {
		DLSSModeUI mode = DLSSUI_Off;
		float sharpness = 0;
	};

	static void ApplySettings(const Settings &settings);
};
