#pragma once
 
#include "UIKeyBindingEntry.h"

#define LOCTEXT_NAMESPACE "Game"

const FText ControlsForward = LOCTEXT("ControlsForward", "Forward");
const FText ControlsBack = LOCTEXT("ControlsBack", "Back");
const FText ControlsRight = LOCTEXT("ControlsRight", "Right");
const FText ControlsLeft = LOCTEXT("ControlsLeft", "Left");

const FText ControlsJump = LOCTEXT("ControlsJump", "Jump");
const FText ControlsSprint = LOCTEXT("ControlsSprint", "Run");

#undef LOCTEXT_NAMESPACE

namespace ControlSettingsConsts
{
	const TMap<FString, TArray<FInputAxis>> DefaultAxis = {
		//{ "MoveForward", {FInputAxis("MoveForward", 1.0f, "W"), FInputAxis("MoveForward", 1.0f, "Up")}},
		//{ "MoveBack", {FInputAxis("MoveForward", -1.0f, "S"), FInputAxis("MoveForward", -1.0f, "Down")} },
		//{ "MoveRight",{FInputAxis("MoveRight", 1.0f, "D"), FInputAxis("MoveRight", 1.0f, "Right")} },
		//{ "MoveLeft", {FInputAxis("MoveRight", -1.0f, "A"), FInputAxis("MoveRight", -1.0f, "Left")} }
	};

	const TMap<FString, TArray<FInputAction>> DefaultAction = {
		{ "Jump", { FInputAction("Jump", false, false, false, false, "SpaceBar")} },
		{ "Run", { FInputAction("Run", false, false, false, false, "LeftShift")} },
	};

	const TArray<FString> RebindableActions = {
		"Jump",
		"Run",
	};

	const TArray<FString> RebindableAxes = {
		//"MoveForward",
		//"MoveRight",
	};

	const TMap<FString, FText> AxisAndActionControls = {
		//{ "MoveForward", ControlsForward },
		//{ "MoveBack", ControlsBack },
		//{ "MoveRight", ControlsRight },
		//{ "MoveLeft", ControlsLeft },

		{ "Jump", ControlsJump },
		{ "Run", ControlsSprint },
	};
}

namespace DefaultSettings
{
	const float DefaultFoV = 90.0F;
	const float DefaultSensitivity = 1.0F;
	const bool DefaultFoVUW = false;
}

UENUM()
enum class ELocale : uint8
{
	En = 0,
	Ru = 1
};