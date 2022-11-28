#include "KeyBindingUtil.h"

#include "HSGameUserSettings.h"
#include "Runtime/Engine/Classes/GameFramework/InputSettings.h"


FInputAction UKeyBindingUtil::GetInputActionFromMouseButtonEvent(const FPointerEvent& MouseEvent)
{
	FInputAction InputAction;

	InputAction.Key = MouseEvent.GetEffectingButton();
	InputAction.KeyAsString = InputAction.Key.GetDisplayName().ToString();

	InputAction.bAlt = MouseEvent.IsAltDown();
	InputAction.bCtrl = MouseEvent.IsControlDown();
	InputAction.bShift = MouseEvent.IsShiftDown();
	InputAction.bCmd = MouseEvent.IsCommandDown();

	return InputAction;
}

/*
*
*/
FInputAxis UKeyBindingUtil::GetInputAxisFromMouseButtonEvent(const FPointerEvent& MouseEvent)
{
	FInputAxis InputAxis;

	InputAxis.Key = MouseEvent.GetEffectingButton();
	InputAxis.KeyAsString = InputAxis.Key.GetDisplayName().ToString();

	InputAxis.Scale = 1;

	return InputAxis;
}

/*
*
*/
FInputAxis UKeyBindingUtil::GetInputAxisFromMouseAxisEvent(const FPointerEvent& MouseEvent)
{
	FInputAxis InputAxis;

	float CurrentHorizontalDistance = FMath::Abs(MouseEvent.GetCursorDelta().X);
	float CurrentVerticalDistance = FMath::Abs(MouseEvent.GetCursorDelta().Y);

	if (CurrentHorizontalDistance > CurrentVerticalDistance)
	{
		InputAxis.Key = FKey(FName("MouseX"));

		InputAxis.Scale = MouseEvent.GetCursorDelta().X >= 0.f ? 1 : -1;
	}
	else
	{
		InputAxis.Key = FKey(FName("MouseY"));

		InputAxis.Scale = MouseEvent.GetCursorDelta().Y >= 0.f ? 1 : -1;
	}


	InputAxis.KeyAsString = InputAxis.Key.GetDisplayName().ToString();

	return InputAxis;
}

/*
*
*/
FInputAction UKeyBindingUtil::GetInputActionFromKeyEvent(const FKeyEvent& KeyEvent)
{
	FInputAction InputAction;

	InputAction.Key = KeyEvent.GetKey();
	InputAction.KeyAsString = InputAction.Key.GetDisplayName().ToString();

	if (KeyEvent.GetKey() != EKeys::LeftAlt && KeyEvent.GetKey() != EKeys::RightAlt)
		InputAction.bAlt = KeyEvent.IsAltDown();
	
	if (KeyEvent.GetKey() != EKeys::LeftControl && KeyEvent.GetKey() != EKeys::RightControl)
		InputAction.bCtrl = KeyEvent.IsControlDown();

	if (KeyEvent.GetKey() != EKeys::LeftShift && KeyEvent.GetKey() != EKeys::RightShift)
		InputAction.bShift = KeyEvent.IsShiftDown();
	
	InputAction.bCmd = KeyEvent.IsCommandDown();

	return InputAction;
}

/*
*
*/
FInputAxis UKeyBindingUtil::GetInputAxisFromKeyEvent(const FKeyEvent& KeyEvent)
{
	FInputAxis InputAxis;

	InputAxis.Key = KeyEvent.GetKey();
	InputAxis.KeyAsString = InputAxis.Key.GetDisplayName().ToString();

	InputAxis.Scale = 1;

	return InputAxis;
}

/*
*
*/
void UKeyBindingUtil::GetAllBindedInputActions(TArray<FInputAction>& InputActions)
{
	InputActions.Empty();

	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return;

	auto& Actions = Settings->GetActionMappings();
	for (const FInputActionKeyMapping& Each : Actions)
		InputActions.Add(FInputAction(Each));
}

void UKeyBindingUtil::GetAllBindedInputAxis(TArray<FInputAxis>& InputAxis)
{
	InputAxis.Empty();

	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return;

	auto& Axis = Settings->GetAxisMappings();
	for (const FInputAxisKeyMapping& Each : Axis)
		InputAxis.Add(FInputAxis(Each));
}

bool UKeyBindingUtil::HasAxisBinding(const FInputAxis& Binding)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	auto& AxisMappings = Settings->GetAxisMappings();
	for (const FInputAxisKeyMapping& Axis : AxisMappings)
	{
		//Search by CurrentBinding
		if (Axis.AxisName.ToString() == Binding.AxisName && Axis.Key == Binding.Key) 
			return true;
	}
	
	return false;
}

bool UKeyBindingUtil::HasAxisBinding(const FString& AxisName, float Scale)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	auto& AxisMappings = Settings->GetAxisMappings();
	for (const FInputAxisKeyMapping& Axis : AxisMappings)
	{
		//Search by CurrentBinding
		if (Axis.AxisName.ToString() == AxisName && Axis.Scale == Scale)
			return true;
	}

	return false;
}

TArray<FInputAxis> UKeyBindingUtil::GetAxisBindings(const FString& ActionName, float Scale)
{
	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return TArray<FInputAxis>();

	auto& AxisMappings = Settings->GetAxisMappings();
	TArray<FInputAxis> Entries;
	for (const FInputAxisKeyMapping& Axis : AxisMappings)
	{
		//Search by CurrentBinding
		if (Axis.AxisName.ToString() == ActionName && Axis.Scale == Scale)
			Entries.Add(Axis);
	}

	return Entries;
}

FInputAxis UKeyBindingUtil::GetAxisBinding(const FString& ActionName, float Scale, const FKey& Key)
{
	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return FInputAxis();

	auto& AxisMappings = Settings->GetAxisMappings();
	for (const FInputAxisKeyMapping& Axis : AxisMappings)
	{
		//Search by CurrentBinding
		if (Axis.AxisName.ToString() == ActionName && Axis.Scale == Scale && Axis.Key == Key)
			return Axis;
	}

	return FInputAxis();
}

bool UKeyBindingUtil::HasActionBinding(const FInputAction& Binding)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	auto& ActionMappings = Settings->GetActionMappings();
	for (auto& Action : ActionMappings)
	{
		//Search by CurrentBinding
		if (Action.ActionName.ToString() == Binding.ActionName && Action.Key == Binding.Key)
			return true;
	}
	
	return false;
}

bool UKeyBindingUtil::HasActionBinding(const FString& ActionName)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	auto& ActionMappings = Settings->GetActionMappings();
	for (auto& Action : ActionMappings)
	{
		//Search by CurrentBinding
		if (Action.ActionName.ToString() == ActionName)
			return true;
	}

	return false;
}

bool UKeyBindingUtil::HasAxisBoundToKey(const TArray<FString>& AxisNames, const FKey& Key)
{
	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return false;

	auto& AxisMappings = Settings->GetAxisMappings();
	for (const FInputAxisKeyMapping& Axis : AxisMappings)
	{
		//Search by CurrentBinding
		if (Axis.Key == Key && AxisNames.Contains(Axis.AxisName.ToString()))
			return true;
	}

	return false;
}

bool UKeyBindingUtil::HasActionBoundToKey(const TArray<FString>& ActionNames, const FKey& Key)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	auto& ActionMappings = Settings->GetActionMappings();
	for (auto& Action : ActionMappings)
	{
		//Search by CurrentBinding
		if (Action.Key == Key && ActionNames.Contains(Action.ActionName.ToString()))
			return true;
	}

	return false;
}

TArray<FInputAction> UKeyBindingUtil::GetActionBindings(const FString& ActionName)
{
	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return TArray<FInputAction>();

	auto& Actions = Settings->GetActionMappings();
	TArray<FInputAction> Entries;
	for (const FInputActionKeyMapping& Each : Actions)
	{
		if (Each.ActionName.ToString() == ActionName)
			Entries.Add(Each);
	}

	return Entries;
}

FInputAction UKeyBindingUtil::GetActionBinding(const FString& ActionName, const FKey& Key)
{
	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) return FInputAction();

	auto& Actions = Settings->GetActionMappings();
	for (const FInputActionKeyMapping& Each : Actions)
	{
		if (Each.ActionName.ToString() == ActionName && Key == Each.Key)
			return Each;
	}

	return FInputAction();
}

/*
*
*/
bool UKeyBindingUtil::ReBindAxisKey(FInputAxis CurrentBinding, FInputAxis NewBinding)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	TArray<FInputAxisKeyMapping> AxisMappings = Settings->GetAxisMappings();

	bool Found = false;
	for (const FInputAxisKeyMapping& Axis : AxisMappings)
	{
		//Search by CurrentBinding
		if (Axis.AxisName.ToString() == CurrentBinding.AxisName && Axis.Key == CurrentBinding.Key) 
		{
			Found = true;

			Settings->RemoveAxisMapping(Axis, false);
			Settings->AddAxisMapping(FInputAxisKeyMapping(FName(*CurrentBinding.AxisName), NewBinding.Key, NewBinding.Scale), true);
			
			break;
		}
	}

	return Found;
}

/*
*
*/
bool UKeyBindingUtil::ReBindActionKey(FInputAction CurrentBinding, FInputAction NewBinding)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	TArray<FInputActionKeyMapping> ActionMappings = Settings->GetActionMappings();

	bool Found = false;
	for (const FInputActionKeyMapping& Action : ActionMappings)
	{
		if (Action.ActionName.ToString() == CurrentBinding.ActionName && Action.Key == CurrentBinding.Key)
		{
			Found = true;
			
			Settings->RemoveActionMapping(Action, false);
			Settings->AddActionMapping(FInputActionKeyMapping(FName(*CurrentBinding.ActionName), NewBinding.Key, NewBinding.bShift, NewBinding.bCtrl, NewBinding.bAlt, NewBinding.bCmd));
			
			break;
		}
	}

	return Found;
}

/*
*
*/
bool UKeyBindingUtil::AddAxisBinding(FString AxisName, FInputAxis NewBinding)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	Settings->AddAxisMapping(FInputAxisKeyMapping(FName(*AxisName), NewBinding.Key, NewBinding.Scale), true);
	
	return true;
}

/*
*
*/
bool UKeyBindingUtil::AddActionBinding(FString ActionName, FInputAction NewBinding)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	Settings->AddActionMapping(FInputActionKeyMapping(FName(*ActionName), NewBinding.Key, NewBinding.bShift, NewBinding.bCtrl, NewBinding.bAlt, NewBinding.bCmd));

	return true;
}

bool UKeyBindingUtil::RemoveAxisBinding(FInputAxis BindingToRemove)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	Settings->RemoveAxisMapping(FInputAxisKeyMapping(FName(*BindingToRemove.AxisName), BindingToRemove.Key, BindingToRemove.Scale), true);

	return true;
}

bool UKeyBindingUtil::RemoveActionBinding(FInputAction BindingToRemove)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	Settings->RemoveActionMapping(FInputActionKeyMapping(FName(*BindingToRemove.ActionName), BindingToRemove.Key, BindingToRemove.bShift, BindingToRemove.bCtrl, BindingToRemove.bAlt, BindingToRemove.bCmd), true);

	return true;
}

bool UKeyBindingUtil::RemoveAxisBindingsByName(const FString& AxisName, float Scale)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	const TArray<FInputAxisKeyMapping>& AxisMappings = Settings->GetAxisMappings();

	TArray<FInputAxisKeyMapping> ToDelete;
	for (const FInputAxisKeyMapping& Axis : AxisMappings)
	{
		//Search by CurrentBinding
		if (Axis.AxisName.ToString() == AxisName && Axis.Scale == Scale)
		{
			ToDelete.Add(FInputAxisKeyMapping(FName(Axis.AxisName), Axis.Key, Axis.Scale));			
		}
	}

	for (int i = 0; i < ToDelete.Num(); i++)
	{
		Settings->RemoveAxisMapping(ToDelete[i], true);
	}

	return true;
}

bool UKeyBindingUtil::RemoveActionBindingsByName(const FString& ActionName)
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	const TArray<FInputActionKeyMapping>& ActionMappings = Settings->GetActionMappings();

	TArray<FInputActionKeyMapping> ToDelete;
	for (const FInputActionKeyMapping& Action : ActionMappings)
	{
		//Search by CurrentBinding
		if (Action.ActionName.ToString() == ActionName)
		{
			ToDelete.Add(FInputActionKeyMapping(FName(Action.ActionName), Action.Key, Action.bShift, Action.bCtrl, Action.bAlt, Action.bCmd));
		}
	}

	for (int i = 0; i < ToDelete.Num(); i++)
	{
		Settings->RemoveActionMapping(ToDelete[i], true);
	}

	//Settings->SaveKeyMappings();

	return true;
}

bool UKeyBindingUtil::ApplyBindingUpdates()
{
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) return false;

	const TArray<FBindingUpdate>& Updates = UHSGameUserSettings::GetHSGameUserSettings()->GetBindingUpdates();

	FInputAxis LastChangedAxis;
	FInputAction LastChangedAction;
	for (int i = 0; i < Updates.Num(); i++)
	{
		if (Updates[i].bIsAxis)
		{
			FInputAxis DefaultBinding = FInputAxis(Updates[i].Name, Updates[i].Scale, Updates[i].DefaultKey);
			FInputAxis NewBinding = FInputAxis(Updates[i].Name, Updates[i].Scale, Updates[i].Key);

			if (!DefaultBinding.Key.IsValid() && NewBinding.Key.IsValid())
			{
				AddAxisBinding(Updates[i].Name, NewBinding);
				LastChangedAxis = NewBinding;
			}
			else if (DefaultBinding.Key.IsValid() && !NewBinding.Key.IsValid() && LastChangedAxis != DefaultBinding)
			{
				RemoveAxisBinding(DefaultBinding);
				LastChangedAxis = DefaultBinding;
			}
			else if (DefaultBinding.Key.IsValid() && NewBinding.Key.IsValid())
			{
				ReBindAxisKey(DefaultBinding, NewBinding);
				LastChangedAxis = NewBinding;
			}
		}
		else
		{
			FInputAction DefaultBinding = FInputAction(Updates[i].Name, false, false, false, false, Updates[i].DefaultKey);
			FInputAction NewBinding = FInputAction(Updates[i].Name, false, false, false, false, Updates[i].Key);

			if (!DefaultBinding.Key.IsValid() && NewBinding.Key.IsValid())
			{
				AddActionBinding(Updates[i].Name, NewBinding);
				LastChangedAction = NewBinding;
			}
			else if (DefaultBinding.Key.IsValid() && !NewBinding.Key.IsValid() && LastChangedAction != DefaultBinding)
			{
				RemoveActionBinding(DefaultBinding);
				LastChangedAction = DefaultBinding;
			}
			else if (DefaultBinding.Key.IsValid() && NewBinding.Key.IsValid())
			{
				ReBindActionKey(DefaultBinding, NewBinding);
				LastChangedAction = NewBinding;
			}
		}
	}

	return true;
}