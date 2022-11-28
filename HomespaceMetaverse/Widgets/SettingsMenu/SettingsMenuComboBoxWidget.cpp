#include "SettingsMenuComboBoxWidget.h"

#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USettingsMenuComboBoxWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ArrowImage->SetRenderTransformAngle(ComboBox->IsOpen() ? 90.0F : 0.0F);
}

void USettingsMenuComboBoxWidget::InitComboBox(const TArray<FText>& Options, int InitialIndex, const FText& InOptionNameText)
{
	ComboBoxOptions = Options;
	UpdateTexts();
	ComboBox->SetSelectedIndex(InitialIndex);
	OptionNameText->SetText(InOptionNameText);
}

void USettingsMenuComboBoxWidget::UpdateTexts()
{
	if(ComboBox->OnSelectionChanged.IsBound())
		ComboBox->OnSelectionChanged.RemoveDynamic(this, &USettingsMenuComboBoxWidget::OnSelectionChanged);

	int Index = ComboBox->GetSelectedIndex();
	ComboBox->ClearOptions();
	for (int i = 0; i < ComboBoxOptions.Num(); i++)
		ComboBox->AddOption(FString("   ") + ComboBoxOptions[i].ToString());

	ComboBox->SetSelectedIndex(Index);
	ComboBox->OnSelectionChanged.AddDynamic(this, &USettingsMenuComboBoxWidget::OnSelectionChanged);
}

UComboBoxString* USettingsMenuComboBoxWidget::GetComboBox()
{
	return ComboBox;
}

int USettingsMenuComboBoxWidget::GetSelectedIndex() const
{
	return ComboBox->GetSelectedIndex();
}

void USettingsMenuComboBoxWidget::SetSelectedIndex(int NewIndex)
{
	ComboBox->SetSelectedIndex(NewIndex);
}

void USettingsMenuComboBoxWidget::OnSelectionChanged(FString Name, ESelectInfo::Type Type)
{
	OnValueChanged.Broadcast();
}
