#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenuOnOffWidget.generated.h"

class UWidgetSwitcher;
class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingsMenuOnOffValueChangedEvent);

UCLASS()
class HOMESPACEMETAVERSE_API USettingsMenuOnOffWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(bool bInitialValue, const FText& InOptionNameText);
	void SetValue(bool Value);
	bool GetValue() const;

	FOnSettingsMenuOnOffValueChangedEvent OnValueChanged;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget)) UTextBlock* OptionNameText;
	UPROPERTY(meta = (BindWidget)) UButton* OnButton;
	UPROPERTY(meta = (BindWidget)) UButton* OffButton;
	UPROPERTY(meta = (BindWidget)) UWidgetSwitcher* OnButtonSwitcher;
	UPROPERTY(meta = (BindWidget)) UWidgetSwitcher* OffButtonSwitcher;

	bool bCurrentValue = false;

	UFUNCTION() void OnButtonClicked();
	UFUNCTION() void OffButtonClicked();
	void Update();

	bool bUiInitialized = false;
};
