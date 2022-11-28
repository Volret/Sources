#include "SettingsMenuSliderWidget.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
 
void USettingsMenuSliderWidget::NativeConstruct()
{
	Super::NativeConstruct(); 

	if (!bUiInitialized)
	{
		bUiInitialized = true;

		ValueSlider->OnValueChanged.AddDynamic(this, &USettingsMenuSliderWidget::OnSliderValueChanged);
	}
}

void USettingsMenuSliderWidget::InitSlider(float InMinValue, float InMaxValue, float InStep, float InStartingValue, const FText& InOptionNameText, int InPrecision)
{ 
	MinValue = InMinValue;
	MaxValue = InMaxValue;
	ValueSlider->SetMinValue(MinValue);
	ValueSlider->SetMaxValue(MaxValue);
	ValueSlider->SetStepSize(InStep);
	ValueSlider->SetValue(InStartingValue);

	Precision = InPrecision;

	SetValue(InStartingValue);
	OptionNameText->SetText(InOptionNameText);
}

USlider* USettingsMenuSliderWidget::GetSlider() 
{
	return ValueSlider;
}

void USettingsMenuSliderWidget::OnSliderValueChanged(float NewVal)
{	
	SetValue(NewVal);
	 
	OnValueChanged.Broadcast();
}

float USettingsMenuSliderWidget::GetValue() const
{
	return CurrentValue;
}

void USettingsMenuSliderWidget::SetValue(float NewVal)
{ 
	CurrentValue = NewVal;
	ValueSlider->SetValue(NewVal);
	BackgroundBar->SetPercent((NewVal - MinValue) / (MaxValue - MinValue));
	
	float Value = FMath::RoundToFloat(NewVal * FMath::Pow(10.0F, Precision)) / FMath::Pow(10.0F, Precision);
	OptionValueText->SetText(FText::FromString(FString::SanitizeFloat(Value, Precision)));
}