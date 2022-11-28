#include "UIKeyBindingEntry.h"

#include "SettingsMenuWidget.h"
#include "SettingsConsts.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UUIKeyBindingEntry::Init(const FText& InActionDisplayName, const FString& ActionName, const FInputAction& InInputActionOne, const FInputAction& InInputActionTwo)
{
	Init();
	bIsAxis = false;
	InputActionOne = InInputActionOne;
	InputActionTwo = InInputActionTwo;

	DisplayAction = InActionDisplayName;
	Action = ActionName;

	TextAction->SetText(DisplayAction);
	UpdateBindingText();
}

void UUIKeyBindingEntry::Init(const FText& InAxisDisplayName, const FString& AxisName, const FInputAxis& InInputAxisOne, const FInputAxis& InInputAxisTwo)
{
	Init();
	bIsAxis = true;
	InputAxisOne = InInputAxisOne;
	InputAxisTwo = InInputAxisTwo;

	DisplayAxis = InAxisDisplayName;
	Axis = AxisName;

	TextAction->SetText(DisplayAxis);
	UpdateBindingText();
}

void UUIKeyBindingEntry::Init()
{
	ButtonBindingOne->OnClicked.AddDynamic(this, &UUIKeyBindingEntry::OnClickOne);
	ButtonBindingTwo->OnClicked.AddDynamic(this, &UUIKeyBindingEntry::OnClickTwo);
}

void UUIKeyBindingEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bIsWainingForInput)
	{
		PanelInputHandler->SetVisibility(ESlateVisibility::Visible);
		ButtonOneSwitcher->SetActiveWidgetIndex(WaitingInputIndex == 1 ? 0 : 1);
		ButtonTwoSwitcher->SetActiveWidgetIndex(WaitingInputIndex == 2 ? 0 : 1);
		ButtonBindingOne->SetVisibility(WaitingInputIndex == 1 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
		ButtonBindingTwo->SetVisibility(WaitingInputIndex == 2 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);

		if (!IsValid(Navigation))
		{
			SetNavigationRuleBase(EUINavigation::Down, EUINavigationRule::Stop);
			SetNavigationRuleBase(EUINavigation::Up, EUINavigationRule::Stop);
		}
	}
	else
	{
		ButtonOneSwitcher->SetActiveWidgetIndex(1);
		ButtonTwoSwitcher->SetActiveWidgetIndex(1);

		ButtonBindingOne->SetVisibility(ESlateVisibility::Visible);
		ButtonBindingTwo->SetVisibility(ESlateVisibility::Visible);
		
		PanelInputHandler->SetVisibility(ESlateVisibility::Hidden);
	}	
}

void UUIKeyBindingEntry::UpdateBindingText()
{
	if (bIsAxis)
	{
		TextBindingOne->SetText(FText::FromString(AxisToString(InputAxisOne)));
		TextBindingTwo->SetText(FText::FromString(AxisToString(InputAxisTwo)));
	}
	else
	{
		TextBindingOne->SetText(FText::FromString(ActionToString(InputActionOne)));
		TextBindingTwo->SetText(FText::FromString(ActionToString(InputActionTwo)));
	}
}

FString UUIKeyBindingEntry::AxisToString(const FInputAxis& InputAxis)
{
	if (!InputAxis.Key.IsValid())
	{
		return FString();
	}

	return InputAxis.KeyAsString;
}

FString UUIKeyBindingEntry::ActionToString(const FInputAction& InputAction)
{
	if (!InputAction.Key.IsValid())
		return FString();
	
	auto Result = InputAction.KeyAsString;
	
	if (InputAction.bAlt)
		Result = "ALT + " + Result;
	
	if (InputAction.bShift)
		Result = "SHIFT + " + Result;
	
	if (InputAction.bCtrl)
		Result = "CTRL + " + Result;
	
	return Result;
}

FReply UUIKeyBindingEntry::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (bIsWainingForInput)
	{
		bIsWainingForInput = false;
		auto Settings = UHSGameUserSettings::GetHSGameUserSettings();
		
		if (InKeyEvent.GetKey() == EKeys::Escape)
			return FReply::Handled();

		if (InKeyEvent.GetKey() == EKeys::BackSpace)
		{
			if (bIsAxis)
			{
				if (WaitingInputIndex == 1 && InputAxisTwo.Key.IsValid())
				{
					Settings->SetBindingUpdate(bIsAxis, 0, InputAxisOne.Scale, InputAxisOne.AxisName, InputAxisTwo.Key, InputAxisOne.Key);
					Settings->SetBindingUpdate(bIsAxis, 1, InputAxisOne.Scale, InputAxisOne.AxisName, FKey(), InputAxisTwo.Key);

					UKeyBindingUtil::RemoveAxisBindingsByName(InputAxisOne.AxisName, InputAxisOne.Scale);

					InputAxisOne = InputAxisTwo;
					InputAxisTwo.Key = FKey();
					InputAxisTwo.KeyAsString = FString();

					UKeyBindingUtil::AddAxisBinding(InputAxisOne.AxisName, InputAxisOne);
				}
				else
				{
					FInputAxis& InputAxis = WaitingInputIndex == 1 ? InputAxisOne : InputAxisTwo;
					Settings->SetBindingUpdate(bIsAxis, WaitingInputIndex - 1, InputAxisOne.Scale, InputAxisOne.AxisName, FKey(), InputAxis.Key);
					UKeyBindingUtil::RemoveAxisBindingsByName(InputAxis.AxisName, InputAxis.Scale);
					InputAxis.Key = FKey();
					InputAxis.KeyAsString = FString();
				}
			}
			else
			{
				if (WaitingInputIndex == 1 && InputActionTwo.Key.IsValid())
				{
					Settings->SetBindingUpdate(bIsAxis, 0, 0.0F, InputActionOne.ActionName, InputActionTwo.Key, InputActionOne.Key);
					Settings->SetBindingUpdate(bIsAxis, 1, 0.0F, InputActionOne.ActionName, FKey(), InputActionTwo.Key);

					UKeyBindingUtil::RemoveActionBinding(InputActionTwo);
					UKeyBindingUtil::ReBindActionKey(InputActionOne, InputActionTwo);

					InputActionOne = InputActionTwo;
					InputActionTwo.Key = FKey();
					InputActionTwo.KeyAsString = FString();
				}
				else
				{
					FInputAction& InputAction = WaitingInputIndex == 1 ? InputActionOne : InputActionTwo;
					Settings->SetBindingUpdate(bIsAxis, WaitingInputIndex - 1, 0.0F, InputActionOne.ActionName, FKey(), InputAction.Key);

					UKeyBindingUtil::RemoveActionBinding(InputAction);
					InputAction.Key = FKey();
					InputAction.KeyAsString = FString();
				}
			}

			UpdateBindingText();
			
			return FReply::Handled();
		}
		
		if (bIsAxis)
		{
			if (UKeyBindingUtil::HasActionBoundToKey(ControlSettingsConsts::RebindableActions, InKeyEvent.GetKey()) ||
				UKeyBindingUtil::HasAxisBoundToKey(ControlSettingsConsts::RebindableAxes, InKeyEvent.GetKey()))
			{
				return FReply::Handled();
			}

			FInputAxis& InputAxis = WaitingInputIndex == 1 ? InputAxisOne : InputAxisTwo;	
			Settings->SetBindingUpdate(bIsAxis, WaitingInputIndex - 1, InputAxisOne.Scale, InputAxisOne.AxisName, InKeyEvent.GetKey(), InputAxis.Key);

			auto NewInputAxis = UKeyBindingUtil::GetInputAxisFromKeyEvent(InKeyEvent);
			NewInputAxis.Scale = InputAxis.Scale;
			NewInputAxis.AxisName = InputAxisOne.AxisName;

			if (InputAxis.Key.IsValid())
			{
				UKeyBindingUtil::ReBindAxisKey(InputAxis, NewInputAxis);
			}
			else
			{
				UKeyBindingUtil::AddAxisBinding(InputAxis.AxisName, NewInputAxis);
			}
			
			InputAxis = NewInputAxis;
		}
		else
		{
			if (UKeyBindingUtil::HasActionBoundToKey(ControlSettingsConsts::RebindableActions, InKeyEvent.GetKey()) ||
				UKeyBindingUtil::HasAxisBoundToKey(ControlSettingsConsts::RebindableAxes, InKeyEvent.GetKey()))
			{
				return FReply::Handled();
			}

			if (InputAxisOne.Key == InKeyEvent.GetKey() || InputAxisTwo.Key == InKeyEvent.GetKey())
			{
				return FReply::Handled();
			}

			FInputAction& InputAction = WaitingInputIndex == 1 ? InputActionOne : InputActionTwo;	
			Settings->SetBindingUpdate(bIsAxis, WaitingInputIndex - 1, 0.0F, InputActionOne.ActionName, InKeyEvent.GetKey(), InputAction.Key);

			auto NewInputAction = UKeyBindingUtil::GetInputActionFromKeyEvent(InKeyEvent);
			NewInputAction.ActionName = InputActionOne.ActionName;

			if (InputAction.Key.IsValid())
			{
				UKeyBindingUtil::ReBindActionKey(InputAction, NewInputAction);
			}
			else
			{
				UKeyBindingUtil::AddActionBinding(InputAction.ActionName, NewInputAction);
			}
			
			InputAction = NewInputAction;
		}

		UpdateBindingText();
		
		return FReply::Handled();
	}	
	
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);	
}

bool UUIKeyBindingEntry::ActionAlreadyBoundToThisKey(const FString& ActionName, const FKey& Key)
{
	if (UKeyBindingUtil::HasActionBinding(ActionName))
	{
		TArray<FInputAction> Actions = UKeyBindingUtil::GetActionBindings(ActionName);
		for (int i = 0; i < Actions.Num(); i++)
		{
			if (Actions[i].Key == Key)
			{
				return true;
			}
		}
	}

	return false;
}

bool UUIKeyBindingEntry::AxisAlreadyBoundToThisKey(const FString& AxisName, const FKey& Key)
{
	if (UKeyBindingUtil::HasAxisBinding(AxisName, 1.0F))
	{
		TArray<FInputAxis> Axes = UKeyBindingUtil::GetAxisBindings(AxisName, 1.0F);
		for (int i = 0; i < Axes.Num(); i++)
		{
			if (Axes[i].Key == Key)
			{
				return true;
			}
		}
	}

	if (UKeyBindingUtil::HasAxisBinding(AxisName, -1.0F))
	{
		TArray<FInputAxis> Axes = UKeyBindingUtil::GetAxisBindings(AxisName, -1.0F);
		for (int i = 0; i < Axes.Num(); i++)
		{
			if (Axes[i].Key == Key)
			{
				return true;
			}
		}
	}

	return false;
}

FReply UUIKeyBindingEntry::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsWainingForInput)
	{
		auto Settings = UHSGameUserSettings::GetHSGameUserSettings();

		if (bIsAxis)
		{
			UE_LOG(LogTemp, Log, TEXT("NO AXIS BINDING FROM MOUSE"))
		}
		else
		{
			auto NewInputAction = UKeyBindingUtil::GetInputActionFromMouseButtonEvent(InMouseEvent);

			if (UKeyBindingUtil::HasActionBoundToKey(ControlSettingsConsts::RebindableActions, NewInputAction.Key))
			{
				return FReply::Handled();
			}

			if (InputAxisOne.Key == NewInputAction.Key || InputAxisTwo.Key == NewInputAction.Key)
			{
				return FReply::Handled();
			}

			FInputAction& InputAction = WaitingInputIndex == 1 ? InputActionOne : InputActionTwo;
			Settings->SetBindingUpdate(false, WaitingInputIndex - 1, 0.0F, InputActionOne.ActionName, InMouseEvent.GetEffectingButton(), InputAction.Key);

			NewInputAction.ActionName = InputActionOne.ActionName;

			if (InputAction.Key.IsValid())
			{
				UKeyBindingUtil::ReBindActionKey(InputAction, NewInputAction);
			}
			else
			{
				UKeyBindingUtil::AddActionBinding(InputAction.ActionName, NewInputAction);
			}

			InputAction = FInputAction(NewInputAction);
		}

		UpdateBindingText();
		
		bIsWainingForInput = false;
		
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UUIKeyBindingEntry::OnClickOne()
{
	bIsWainingForInput = true;
	WaitingInputIndex = 1;

	SetFocus();
	SettingsMenuWidget->UnfocusOtherBindingEntries(this);
}


void UUIKeyBindingEntry::OnClickTwo()
{
	bIsWainingForInput = true;
	WaitingInputIndex = 2;

	SetFocus();
	SettingsMenuWidget->UnfocusOtherBindingEntries(this);
}

void UUIKeyBindingEntry::RestoreDefaults()
{
	auto Settings = UHSGameUserSettings::GetHSGameUserSettings();

	if (bIsAxis)
	{
		FString AxisName = Axis;
		float Scale = InputAxisOne.Scale;

		if (AxisName == "MoveForward" && Scale < 0)
			AxisName = "MoveBack";
		if (AxisName == "MoveRight" && Scale < 0)
			AxisName = "MoveLeft";

		UKeyBindingUtil::RemoveAxisBindingsByName(Axis, Scale);

		// Set Default Axis
		if (auto Ax = ControlSettingsConsts::DefaultAxis.Find(AxisName))
		{
			if (Ax->Num() > 0)
			{
				auto DefaultKey1 = (*Ax)[0].Key;

				FInputAxis DefaultAxis1 = (*Ax)[0];
				UKeyBindingUtil::AddAxisBinding(Axis, DefaultAxis1);
				InputAxisOne = DefaultAxis1;
				InputAxisTwo = FInputAxis();
				if (Ax->Num() > 1)
				{
					auto DefaultKey2 = (*Ax)[1].Key;

					FInputAxis DefaultAxis2 = (*Ax)[1];
					UKeyBindingUtil::AddAxisBinding(Axis, DefaultAxis2);
					InputAxisTwo = DefaultAxis2;
				}
			}
		}
	}
	else
	{
		FString ActionName = Action;
		UKeyBindingUtil::RemoveActionBindingsByName(Action);

		// Set Default Actions
		if (auto Ac = ControlSettingsConsts::DefaultAction.Find(ActionName))
		{
			if (Ac->Num() > 0)
			{
				auto DefaultKey1 = (*Ac)[0].Key;

				FInputAction DefaultAction1(Action, false, false, false, false, DefaultKey1);
				UKeyBindingUtil::AddActionBinding(Action, DefaultAction1);
				InputActionOne = DefaultAction1;
				InputActionTwo = FInputAction();
				if (Ac->Num() > 1)
				{
					auto DefaultKey2 = (*Ac)[1].Key;

					FInputAction DefaultAction2(Action, false, false, false, false, DefaultKey2);
					UKeyBindingUtil::AddActionBinding(Action, DefaultAction2);
					InputActionTwo = DefaultAction2;
				}
			}
		}
	}

	UpdateBindingText();
}
