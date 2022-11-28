#include "SettingsMenuOnOffWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void USettingsMenuOnOffWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!bUiInitialized)
	{
		bUiInitialized = true;

		OnButton->OnClicked.AddDynamic(this, &USettingsMenuOnOffWidget::OnButtonClicked);
		OffButton->OnClicked.AddDynamic(this, &USettingsMenuOnOffWidget::OffButtonClicked);
	}
}

void USettingsMenuOnOffWidget::Init(bool bInitialValue, const FText& InOptionNameText)
{
	SetValue(bInitialValue);
	OptionNameText->SetText(InOptionNameText);
}

void USettingsMenuOnOffWidget::SetValue(bool Value)
{
	bCurrentValue = Value;
	Update();
}

bool USettingsMenuOnOffWidget::GetValue() const
{
	return bCurrentValue;
}

void USettingsMenuOnOffWidget::OnButtonClicked()
{
	if (bCurrentValue)
		return;

	bCurrentValue = true;
	OnValueChanged.Broadcast();
	Update();
}

void USettingsMenuOnOffWidget::OffButtonClicked()
{
	if (!bCurrentValue)
		return;

	bCurrentValue = false;
	OnValueChanged.Broadcast();
	Update();
}

void USettingsMenuOnOffWidget::Update()
{
	OnButtonSwitcher->SetActiveWidgetIndex(bCurrentValue ? 0 : 1);
	OffButtonSwitcher->SetActiveWidgetIndex(bCurrentValue ? 1 : 0);
}