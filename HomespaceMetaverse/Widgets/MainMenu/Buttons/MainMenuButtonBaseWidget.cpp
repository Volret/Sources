#include "MainMenuButtonBaseWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"

void UMainMenuButtonBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!bUiInitialized)
	{
		bUiInitialized = true;

		Button->OnClicked.AddDynamic(this, &UMainMenuButtonBaseWidget::OnButtonClicked);
		Button->OnHovered.AddDynamic(this, &UMainMenuButtonBaseWidget::OnButtonHovered);
		Button->OnUnhovered.AddDynamic(this, &UMainMenuButtonBaseWidget::OnButtonUnhovered);
		Button->OnPressed.AddDynamic(this, &UMainMenuButtonBaseWidget::OnButtonPressed);
		Button->OnReleased.AddDynamic(this, &UMainMenuButtonBaseWidget::OnButtonReleased);
	}

	OnButtonUnhovered();
}

void UMainMenuButtonBaseWidget::SetActiveState(bool bIsActive)
{
	if (ButtonStateSwitcher->GetChildrenCount() > 1)
	{
		ButtonStateSwitcher->SetActiveWidgetIndex(bIsActive ? 1 : 0);
	}

	ButtonText->SetColorAndOpacity(bIsActive ? ActiveTextColor : InactiveTextColor);

	bIsButtonActive = bIsActive;
}

void UMainMenuButtonBaseWidget::SetButtonText(FText Text)
{
	ButtonText->SetText(Text);
}

FText UMainMenuButtonBaseWidget::GetCurrentText()
{
	return ButtonText->GetText();
}

void UMainMenuButtonBaseWidget::OnButtonClicked()
{
	ClickSound.Play(this);
}

void UMainMenuButtonBaseWidget::OnButtonHovered()
{
	if (!bIsButtonActive)
	{
		ButtonText->SetColorAndOpacity(HoveredTextColor);
		HoverSound.Play(this);
	}
}

void UMainMenuButtonBaseWidget::OnButtonUnhovered()
{
	if (!bIsButtonActive)
		ButtonText->SetColorAndOpacity(InactiveTextColor);
}

void UMainMenuButtonBaseWidget::OnButtonPressed()
{

}

void UMainMenuButtonBaseWidget::OnButtonReleased()
{

}