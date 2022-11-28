#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenuSliderWidget.generated.h"

class USlider;
class UTextBlock;
class UProgressBar;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSliderValueChangedEvent);

UCLASS()
class HOMESPACEMETAVERSE_API USettingsMenuSliderWidget : public UUserWidget
{
	GENERATED_BODY()

public: 
	virtual void NativeConstruct() override;
	void InitSlider(float InMinValue, float InMaxValue, float InStep, float InStartingValue, const FText& InOptionNameText, int InPrecision = 0);

	float GetValue() const;
	void SetValue(float NewVal);
	USlider* GetSlider(); 

	FOnSliderValueChangedEvent OnValueChanged;

private:
	UFUNCTION() void OnSliderValueChanged(float NewVal);
	
	float CurrentValue = 0.0F;
	float MinValue = 0.0F;
	float MaxValue = 0.0F;
	int Precision;

	UPROPERTY(meta = (BindWidget)) USlider* ValueSlider;
	UPROPERTY(meta = (BindWidget)) UProgressBar* BackgroundBar;
	UPROPERTY(meta = (BindWidget)) UTextBlock* OptionNameText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* OptionValueText;

	bool bUiInitialized = false;
};
