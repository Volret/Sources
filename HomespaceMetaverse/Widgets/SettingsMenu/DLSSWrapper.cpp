#ifdef HS_WITH_DLSS

#include "DLSSWrapper.h"
#include "SettingsConsts.h"

#define LOCTEXT_NAMESPACE "Game"

const FText TextDLSSDisabled = LOCTEXT("DLSSDisabled", "Disabled");
const FText TextDLSSAuto = LOCTEXT("DLSSAuto", "Auto");
const FText TextDLSSQuality1 = LOCTEXT("DLSSQuality1", "Ultra Performance");		// DLSS' Ultra Performance
const FText TextDLSSQuality2 = LOCTEXT("DLSSQuality2", "Performance");	// DLSS' Performance
const FText TextDLSSQuality3 = LOCTEXT("DLSSQuality3", "Balanced");		// DLSS' Balanced
const FText TextDLSSQuality4 = LOCTEXT("DLSSQuality4", "Quality");	// DLSS' Quality
const FText TextDLSSQuality5 = LOCTEXT("DLSSQuality5", "Ultra Quality");  // DLSS' Ultra Quality

#undef LOCTEXT_NAMESPACE


UDLSSMode DLSSWrapper::DLSSUI2UDLSS(DLSSModeUI modeui) 
{
	switch (modeui) 
	{
	case DLSSUI_Off: return UDLSSMode::Off;
	case DLSSUI_Auto: return UDLSSMode::Off;
	case DLSSUI_UltraPerformance: return UDLSSMode::UltraPerformance;
	case DLSSUI_Performance: return UDLSSMode::Performance;
	case DLSSUI_Balanced: return UDLSSMode::Balanced;
	case DLSSUI_Quality: return UDLSSMode::Quality;
	case DLSSUI_UltraQuality: return UDLSSMode::UltraQuality;
	}
	return UDLSSMode::Off;
}

DLSSModeUI DLSSWrapper::UDLSS2DLSSUI(UDLSSMode mode) 
{
	for (int i = 0; i < DLSSUI_N; i++) 
	{
		if (DLSSUI2UDLSS(DLSSModeUI(i)) == mode) 
		{
			return DLSSModeUI(i);
		}
	}
	return DLSSUI_Off;
}

TArray<FText> DLSSWrapper::GetAvailableDlssModes() 
{
	TArray<FText> AllModes = {
		 TextDLSSDisabled,
		 TextDLSSAuto,
		 TextDLSSQuality1,
		 TextDLSSQuality2,
		 TextDLSSQuality3,
		 TextDLSSQuality4,
		 TextDLSSQuality5
	};
	if (AllModes.Num() != DLSSUI_N) {
		UE_LOG(LogTemp, Error, TEXT("DLSSWrapper::GetAvailableDlssModes - internal error, inconsistent DLSS modes enumeration"));
	}

	TArray<FText> Modes;
	for (int i = 0; i < AllModes.Num(); i++)
	{
		if (ModeIsSupported(UDLSSMode(i)))
		{
			Modes.Push(AllModes[i]);
		}
	}
	return Modes;
} 

bool DLSSWrapper::ModeIsSupported(UDLSSMode mode) 
{
	int i = int(mode);
	return (i == DLSSUI_Off || i == DLSSUI_Auto || UDLSSLibrary::IsDLSSModeSupported(DLSSUI2UDLSS(DLSSModeUI(i))));
}

int DLSSWrapper::ModeToListIndex(UDLSSMode mode) 
{	
	int index = int(mode);
	for (int i = 0; i <= int(mode); i++)
	{
		if (!ModeIsSupported(UDLSSMode(i)))
		{
			index--;
		}
	}
	if (index < 0) 
		index = 0;
	return index;
}

bool DLSSWrapper::IsDLSSSupported() 
{
	return UDLSSLibrary::IsDLSSSupported();
}

void DLSSWrapper::ApplySettings(const DLSSWrapper::Settings& settings) 
{
	//UE_LOG(LogTemp, Warning, TEXT("DLSS modeui: %d, sharpness %f"), settings.mode, settings.sharpness);
	if (IsDLSSSupported()) 
	{
		UDLSSLibrary::SetDLSSMode(DLSSUI2UDLSS(DLSSModeUI(settings.mode)));
		UDLSSLibrary::SetDLSSSharpness(settings.sharpness);
	}
}

#endif