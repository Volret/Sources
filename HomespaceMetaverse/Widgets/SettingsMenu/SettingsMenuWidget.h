#pragma once

#include "SettingsConsts.h"
#include "HSGameUserSettings.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenuWidget.generated.h"

class UButton;
class UScrollBox;
class UHorizontalBox;
class UWidgetSwitcher;
class UUIKeyBindingEntry;
class USettingsMenuOnOffWidget;
class USettingsMenuSliderWidget;
class USettingsMenuComboBoxWidget; 
class UMainMenuButtonBaseWidget;
 
UCLASS()
class HOMESPACEMETAVERSE_API USettingsMenuWidget : public UUserWidget
{
	GENERATED_BODY()	

public:
	void UnfocusOtherBindingEntries(UUIKeyBindingEntry* LastClickedBindingEntry);
	void SetCurrentTabButtonActiveIndex(int Index);

protected:
	virtual void NativeConstruct();

	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUIKeyBindingEntry> KeyBindingEntryBlueprint;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<USettingsMenuOnOffWidget> SettingsMenuOnOffWidgetBlueprint;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<USettingsMenuSliderWidget> SettingsMenuSliderWidgetBlueprint;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<USettingsMenuComboBoxWidget> SettingsMenuComboBoxWidgetBlueprint;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly) UMainMenuButtonBaseWidget* CustomGraphicsPresetButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly) UMainMenuButtonBaseWidget* LowGraphicsPresetButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly) UMainMenuButtonBaseWidget* MediumGraphicsPresetButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly) UMainMenuButtonBaseWidget* HighGraphicsPresetButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly) UMainMenuButtonBaseWidget* UltraGraphicsPresetButton;

	UPROPERTY(EditDefaultsOnly, Category = Audio) USoundClass* SCL_Overall;
	UPROPERTY(EditDefaultsOnly, Category = Audio) USoundClass* SCL_Music;
	UPROPERTY(EditDefaultsOnly, Category = Audio) USoundClass* SCL_SFX;

private:
	UPROPERTY(Transient) TArray<UUIKeyBindingEntry*> BindingEntries;

	UPROPERTY(Transient) USettingsMenuSliderWidget* SensitivitySlider;
	UPROPERTY(Transient) USettingsMenuSliderWidget* FoVSlider;

	UPROPERTY(Transient) USettingsMenuComboBoxWidget* ResolutionComboBox;
	UPROPERTY(Transient) USettingsMenuOnOffWidget* FullscreenOnOff;
	UPROPERTY(Transient) USettingsMenuComboBoxWidget* AAQualityComboBox;
	UPROPERTY(Transient) USettingsMenuComboBoxWidget* PostProcessingQualityComboBox;
	UPROPERTY(Transient) USettingsMenuComboBoxWidget* ShadowsQualityComboBox;
	UPROPERTY(Transient) USettingsMenuComboBoxWidget* TexturesQualityComboBox;
	UPROPERTY(Transient) USettingsMenuComboBoxWidget* AnisotropyQualityComboBox;
	UPROPERTY(Transient) USettingsMenuComboBoxWidget* EffectsQualityComboBox;
	UPROPERTY(Transient) USettingsMenuSliderWidget* GammaSlider;
	UPROPERTY(Transient) USettingsMenuSliderWidget* ScreenResolutionPercentSlider;
	UPROPERTY(Transient) USettingsMenuOnOffWidget* SSLROnOff;
	UPROPERTY(Transient) USettingsMenuOnOffWidget* GlobalAOOnOff;
	UPROPERTY(Transient) USettingsMenuOnOffWidget* VSyncOnOff;
	UPROPERTY(Transient) USettingsMenuComboBoxWidget* MotionBlurComboBox;
	UPROPERTY(Transient) USettingsMenuComboBoxWidget* AOComboBox;
	UPROPERTY(Transient) USettingsMenuComboBoxWidget* DLSSModeComboBox;
	UPROPERTY(Transient) USettingsMenuSliderWidget* DLSSSharpnessSlider;
	UPROPERTY(Transient) USettingsMenuSliderWidget* ShadowsDrawDistanceMultSlider;

	UPROPERTY(meta = (BindWidget)) UWidgetSwitcher* TabSwitcher;
	UPROPERTY(meta = (BindWidget)) UHorizontalBox* TabButtonsBox;

	UPROPERTY(meta = (BindWidget)) UScrollBox* GameTabBox;
	UPROPERTY(meta = (BindWidget)) UScrollBox* InterfaceTabBox;
	UPROPERTY(meta = (BindWidget)) UScrollBox* ScrollBoxKeyBindings;
	UPROPERTY(meta = (BindWidget)) UScrollBox* GraphicsTabBox;
	UPROPERTY(meta = (BindWidget)) UScrollBox* SoundTabBox;

	UPROPERTY(meta = (BindWidget)) UButton* RestoreDefaultsButton;
	UPROPERTY(meta = (BindWidget)) UButton* BackButton;

	UPROPERTY(Transient) TArray<USettingsMenuComboBoxWidget*> ComboBoxes;

	int CurrentTabButtonActiveIndex = 0;

	void InitGameSettings();
	void InitControlSettings();
	void InitGraphicsSettings();
	void InitResolutionSetting();

	void SetGraphics(const FGraphicsSettings& Quality);
	void UpdateGraphics(const FGraphicsSettings& OldQuality, const FGraphicsSettings& NewQuality);
	
#ifdef HS_WITH_DLSS
	void ApplyDLSSSettings(const FGraphicsSettings& Quality, bool CheckChanging = false, const FGraphicsSettings &OldQuality = FGraphicsSettings());
	void UpdateDLSSWidgets(const FGraphicsSettings& Quality); 
#endif

	UFUNCTION() void SetLowGraphicsQuality();
	UFUNCTION() void SetMediumGraphicsQuality();
	UFUNCTION() void SetHighGraphicsQuality();
	UFUNCTION() void SetUltraGraphicsQuality();
	void UpdateQualityIndex(int Index);

	UFUNCTION() void SensitivityValueChanged(float NewVal);
	UFUNCTION() void DefaultFoVValueChanged(float NewVal);
	UFUNCTION() void GraphicsQualityChanged();
	UFUNCTION() void ResolutionChanged();
	UFUNCTION() void BackButtonClicked();
	UFUNCTION() void RestoreDefaults();
	UFUNCTION() void RestoreDefaultsAll();
	UFUNCTION() void RestoreDefaultsGame();
	UFUNCTION() void RestoreDefaultsInterface();
	UFUNCTION() void RestoreDefaultsControls();
	UFUNCTION() void RestoreDefaultsGraphics();

	bool GraphicsQualityChangedEnabled = true;		// Preventing GraphicsQualityChanged() execution during UpdateGraphicsWidgetsFromQuality()
	void UpdateGraphicsWidgetsFromQuality(const FGraphicsSettings& GraphicsSettings);

	USettingsMenuSliderWidget* CreateSlider(float InMinValue, float InMaxValue, float InStep, float InStartingValue, const FText& OptionName, int Precision, UScrollBox* Box);
	USettingsMenuComboBoxWidget* CreateComboBox(const TArray<FText>& Options, int InitialIndex, const FText& OptionName, UScrollBox* Box);
	USettingsMenuOnOffWidget* CreateOnOff(bool bInitialValue, const FText& OptionName, UScrollBox* Box);

	TArray<FIntPoint> Resolutions = {
		FIntPoint(1024, 768),
		FIntPoint(1280, 720),
		FIntPoint(1280, 800),
		FIntPoint(1280, 1024),
		FIntPoint(1360, 768),
		FIntPoint(1366, 768),
		FIntPoint(1440, 900),
		FIntPoint(1536, 864),
		FIntPoint(1600, 900),
		FIntPoint(1680, 1050),
		FIntPoint(1920, 1080),
		FIntPoint(1920, 1200),
		FIntPoint(2048, 1152),
		FIntPoint(2048, 1536),
		FIntPoint(2560, 1080),
		FIntPoint(2560, 1440),
		FIntPoint(3440, 1440),
		FIntPoint(3840, 2160)
	};

	bool bUiInitialized = false;
};
