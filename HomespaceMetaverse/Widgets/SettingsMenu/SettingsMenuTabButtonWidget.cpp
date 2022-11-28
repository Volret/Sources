#include "SettingsMenuTabButtonWidget.h"

#include "SettingsMenuWidget.h"

void USettingsMenuTabButtonWidget::OnButtonClicked()
{
	SettingsMenuWidget->SetCurrentTabButtonActiveIndex(ButtonIndex);
}