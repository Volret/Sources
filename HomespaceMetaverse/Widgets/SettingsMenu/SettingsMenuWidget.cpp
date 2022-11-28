#include "SettingsMenuWidget.h"

#include "HSLocalPlayer.h"
#include "UIKeyBindingEntry.h"
#include "SettingsMenuOnOffWidget.h"
#include "SettingsMenuSliderWidget.h"
#include "SettingsMenuComboBoxWidget.h"
#include "SettingsMenuTabButtonWidget.h"
#include "HSPlayerCharacter.h"
#include "HSPlayerController.h"

#include "Components/Slider.h"
#include "Components/HorizontalBox.h"
#include "Components/ComboBoxString.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBoxSlot.h"

#ifdef HS_WITH_DLSS
#include "DLSSWrapper.h"
#endif

#define LOCTEXT_NAMESPACE "Game"
const FText TextLanguage = LOCTEXT("Language", "Language");
const FText TextEnglishLanguage = LOCTEXT("EnglishLanguage", "English");
const FText TextRussianLanguage = LOCTEXT("RussianLanguage", "Russian");
const FText TextCrosshairSpecial = LOCTEXT("CrosshairSpecial", "Use special crosshairs for each weapon");
const FText TextCrosshairAmmoBar = LOCTEXT("CrosshairAmmoBar", "Show ammo bar under crosshair");

const FText TextOverallAudioBusVolume = LOCTEXT("OverallAudioBusVolume", "Overall Audio Volume");
const FText TextMusicAudioBusVolume = LOCTEXT("MusicAudioBusVolume", "Music Audio Volume");
const FText TextSFXAudioBusVolume = LOCTEXT("SFXAudioBusVolume", "SFX Audio Volume");

const FText TextSensitivity = LOCTEXT("Sensitivity", "Sensitivity");
const FText TextFoV = LOCTEXT("FoV", "FoV");
const FText TextFoVUW = LOCTEXT("FoVUW", "Ultrawide monitor FoV");
const FText TextAllowBackfill = LOCTEXT("AllowBackfill", "Allow backfilling");
const FText TextOnlyFriendsCanInviteToSquads = LOCTEXT("OnlyFriendsCanInviteToSquads", "Get invitations to squads only from friends");
const FText TextOnlyFriendsCanSendPrivateMessages = LOCTEXT("OnlyFriendsCanSendPrivateMessages", "Get private messages only from friends");

const FText TextResolution = LOCTEXT("Resolution", "Resolution");
const FText TextFullscreen = LOCTEXT("Fullscreen", "Fullscreen");
const FText TextTextureResolution = LOCTEXT("TextureResolution", "Texture resolution");
const FText TextSSLR = LOCTEXT("SSLR", "SSLR");
const FText TextEnabled = LOCTEXT("Enabled", "Enabled");
const FText TextDisabled = LOCTEXT("Disabled", "Disabled");
const FText TextCustom = LOCTEXT("Custom", "Custom");
const FText TextLow = LOCTEXT("Low", "Low");
const FText TextMedium = LOCTEXT("Medium", "Medium");
const FText TextHigh = LOCTEXT("High", "High");
const FText TextUltra = LOCTEXT("Ultra", "Ultra");
const FText TextShadows = LOCTEXT("Shadows", "Shadows");
const FText TextShadowsDrawDistance = LOCTEXT("ShadowsDrawDistance", "Shadows draw distance");
const FText TextRenderingQuality = LOCTEXT("RenderingQuality", "Rendering resolution quality %");
const FText TextPostProcessing = LOCTEXT("PostProcessing", "Post - Processing");
const FText TextParticleCullingScalability = LOCTEXT("ParticleCullingScalability", "Particle culling scalability");
const FText TextMotionBlur = LOCTEXT("MotionBlur", "Motion Blur");
const FText TextGrassCulling = LOCTEXT("GrassCulling", "Grass culling");
const FText TextGlobalAO = LOCTEXT("GlobalAO", "Global AO");
const FText TextVSync = LOCTEXT("VSync", "Vertical Synchronization");
const FText TextGamma = LOCTEXT("Gamma", "Gamma");
const FText TextEffects = LOCTEXT("Effects", "Effects");
const FText TextDynamicWater = LOCTEXT("DynamicWater", "Dynamic water");
const FText TextHitAnimation = LOCTEXT("HitAnimation", "Enable hit animation");
const FText TextWeaponPhysics = LOCTEXT("WeaponPhysics", "Enable weapon physics");

#ifdef HS_WITH_DLSS
const FText TextDLSSSharpness = LOCTEXT("DLSSSharpness", "DLSS Sharpness");
const FText TextDLSSMode = LOCTEXT("DLSSMode", "DLSS Mode");
#endif

const FText TextAmbientOcclusion = LOCTEXT("AmbientOcclusion", "Ambient occlusion");
const FText TextAnisotropyLevel = LOCTEXT("AnisotropyLevel", "Anisotropy level");
const FText TextAntiAliasing = LOCTEXT("AntiAliasing", "Anti - Aliasing");
const FText TextUnlimited = LOCTEXT("Unlimited", "Unlimited");
const FText TextSkeletalMesh = LOCTEXT("TextSkeletalMesh", "Skeletal meshes quality");
const FText TextDecorators = LOCTEXT("TextDecorators", "Decorators quality");
#undef LOCTEXT_NAMESPACE

template<typename ValueType> void AcceptSetting(IConsoleVariable* CVar, const ValueType& OldValue, const ValueType& NewValue)
{
	if (OldValue != NewValue)
	{
		CVar->Set(NewValue);
	}
}

void USettingsMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!bUiInitialized)
	{
		bUiInitialized = true;

		InitGameSettings();
		InitControlSettings();
		InitGraphicsSettings();

		for (int i = 0; i < TabButtonsBox->GetChildrenCount(); i++)
		{
			USettingsMenuTabButtonWidget* Button = Cast<USettingsMenuTabButtonWidget>(TabButtonsBox->GetChildAt(i));
			Button->SetSettingsMenuWidget(this);
			Button->SetButtonIndex(i);
			Button->SetActiveState(i == 0);
		}

		RestoreDefaultsButton->OnClicked.AddDynamic(this, &USettingsMenuWidget::RestoreDefaults);
		BackButton->OnClicked.AddDynamic(this, &USettingsMenuWidget::BackButtonClicked);
	}
}

void USettingsMenuWidget::SetCurrentTabButtonActiveIndex(int Index)
{
	if (CurrentTabButtonActiveIndex == Index)
		return;

	CurrentTabButtonActiveIndex = Index;
	for (int i = 0; i < TabButtonsBox->GetChildrenCount(); i++)
	{
		USettingsMenuTabButtonWidget* Button = Cast<USettingsMenuTabButtonWidget>(TabButtonsBox->GetChildAt(i));
		Button->SetActiveState(i == CurrentTabButtonActiveIndex);
	}

	TabSwitcher->SetActiveWidgetIndex(CurrentTabButtonActiveIndex);
}

void USettingsMenuWidget::InitControlSettings()
{
	UKeyBindingUtil::ApplyBindingUpdates();

	const TArray<FBindingUpdate>& Updates = UHSGameUserSettings::GetHSGameUserSettings()->GetBindingUpdates();

	TArray<FInputAxis> InputAxises;
	UKeyBindingUtil::GetAllBindedInputAxis(InputAxises);

	int Index = 0;

	if (IsValid(ScrollBoxKeyBindings))
		ScrollBoxKeyBindings->ClearChildren();

	for (auto Axis : ControlSettingsConsts::RebindableAxes)
	{
		for (auto Scale : { 1, -1 })
		{
			auto SelectedAxes = InputAxises.FilterByPredicate(
				[&Axis, &Scale](const FInputAxis& InputAxis)
				{
					return InputAxis.AxisName == Axis && FMath::Sign(InputAxis.Scale) == FMath::Sign(Scale) && !InputAxis.Key.GetFName().ToString().Contains("Gamepad");
				});

			// Sort By Index (For Updated bindings, it solves shuffling problem)
			SelectedAxes.Sort([&Updates](const FInputAxis& First, const FInputAxis& Second) -> bool
			{

				auto ContainsInUpdatesPredicate1 = [&First](const FBindingUpdate& Value)
				{
					return Value.bIsAxis && First.AxisName == Value.Name && First.Scale == Value.Scale && First.Key == Value.Key;
				};

				auto ContainsInUpdatesPredicate2 = [&Second](const FBindingUpdate& Value)
				{
					return  Value.bIsAxis && Second.AxisName == Value.Name && Second.Scale == Value.Scale && Second.Key == Value.Key;
				};

				auto Update1 = Updates.FindByPredicate(ContainsInUpdatesPredicate1);
				auto Update2 = Updates.FindByPredicate(ContainsInUpdatesPredicate2);

				return (Update1 && Update2) && (Update1->Index < Update2->Index);
			});

			FInputAxis AxisOne;
			FInputAxis AxisTwo;

			int FirstIndex = 0;
			int SecondIndex = 0;

			auto AxisName = Axis;
			if (Axis == "MoveForward" && Scale < 0)
				AxisName = "MoveBack";
			if (Axis == "MoveRight" && Scale < 0)
				AxisName = "MoveLeft";


			if (auto Ax = ControlSettingsConsts::DefaultAxis.Find(AxisName))
			{
				if (SelectedAxes.Num() > 1)
				{
					auto Axis0 = (*Ax)[0];
					// Sort By Index (For Default bindings, it solves shuffling problem)
					FirstIndex = FMath::Max(SelectedAxes.IndexOfByPredicate(
						[&Axis0, &Axis](const FInputAxis& Value)
						{
							return Axis == Axis0.AxisName && Value.Scale == Axis0.Scale && Value.Key == Axis0.Key;
						}), 0);

					SecondIndex = (FirstIndex + 1) % 2;
				}

				if (SelectedAxes.Num() == 0)
				{
					AxisOne = FInputAxis(Axis, Scale, FKey());
					AxisTwo = FInputAxis(Axis, Scale, FKey());
				}
				else
				{
					AxisOne = SelectedAxes[FirstIndex];
					if (SelectedAxes.Num() > 1)
					{
						AxisTwo = SelectedAxes[SecondIndex];
					}
					else
					{
						AxisTwo.AxisName = AxisOne.AxisName;
						AxisTwo.Scale = AxisOne.Scale;
					}
				}

				FText AxisDisplayName = ControlSettingsConsts::AxisAndActionControls.Contains(AxisName) ? ControlSettingsConsts::AxisAndActionControls[AxisName] : FText();
				FName WidgetName = FName(FString::Printf(TEXT("%i"), Index++));

				auto InputEntry = CreateWidget<UUIKeyBindingEntry>(ScrollBoxKeyBindings, KeyBindingEntryBlueprint);
				InputEntry->SetSettingsMenuWidget(this);
				InputEntry->Init(AxisDisplayName, Axis, AxisOne, AxisTwo);
				Cast<UScrollBoxSlot>(ScrollBoxKeyBindings->AddChild(InputEntry))->SetPadding(FMargin(0.0F, 0.0F, 0.0F, 4.0F));
				BindingEntries.Add(InputEntry);
			}
		}
	}

	TArray<FInputAction> InputActions;
	UKeyBindingUtil::GetAllBindedInputActions(InputActions);

	for (auto Action : ControlSettingsConsts::RebindableActions)
	{
		auto SelectedActions = InputActions.FilterByPredicate(
			[&Action](const FInputAction& InputAction)
			{
				return InputAction.ActionName == Action && InputAction.Key.IsValid() && !InputAction.Key.GetFName().ToString().Contains("Gamepad");
			});

		FInputAction ActionOne;
		FInputAction ActionTwo;

		int FirstIndex = 0;
		int SecondIndex = 0;

		if (auto Ac = ControlSettingsConsts::DefaultAction.Find(Action))
		{
			if (SelectedActions.Num() > 1)
			{
				auto Action0 = (*Ac)[0];
				// Sort By Index (For Default bindings, it solves shuffling problem)
				FirstIndex = FMath::Max(SelectedActions.IndexOfByPredicate(
					[&Action0](const FInputAction& Value)
					{
						return Value.ActionName == Action0.ActionName && Value.Key == Action0.Key;
					}), 0);

				SecondIndex = (FirstIndex + 1) % 2;
			}

			if (SelectedActions.Num() == 0)
			{
				ActionOne = FInputAction(Action, false, false, false, false, FKey());
				ActionTwo = FInputAction(Action, false, false, false, false, FKey());
			}
			else
			{
				ActionOne = SelectedActions[FirstIndex];
				if (SelectedActions.Num() > 1)
					ActionTwo = SelectedActions[SecondIndex];
				else
				{
					ActionTwo.ActionName = ActionOne.ActionName;
				}
			}

			FText ActionName = ControlSettingsConsts::AxisAndActionControls.Contains(Action) ? ControlSettingsConsts::AxisAndActionControls[Action] : FText();
			FName WidgetName = FName(FString::Printf(TEXT("%i"), Index++));

			auto InputEntry = Cast<UUIKeyBindingEntry>(CreateWidgetInstance(*ScrollBoxKeyBindings, KeyBindingEntryBlueprint, WidgetName));
			InputEntry->SetSettingsMenuWidget(this);
			InputEntry->Init(ActionName, Action, ActionOne, ActionTwo);
			Cast<UScrollBoxSlot>(ScrollBoxKeyBindings->AddChild(InputEntry))->SetPadding(FMargin(0.0F, 0.0F, 0.0F, 4.0F));
			BindingEntries.Add(InputEntry);
		}
	}
}

void USettingsMenuWidget::UnfocusOtherBindingEntries(UUIKeyBindingEntry* LastClickedBindingEntry)
{
	for (int i = 0; i < BindingEntries.Num(); i++)
	{
		if (BindingEntries[i] == LastClickedBindingEntry)
		{
			continue;
		}

		BindingEntries[i]->ClearFocus();
	}
}

void USettingsMenuWidget::InitGameSettings()
{
	UHSLocalPlayer* Player = GetOwningLocalPlayer<UHSLocalPlayer>();

	if (IsValid(SensitivitySlider)) {
		SensitivitySlider->RemoveFromParent();
	}

	SensitivitySlider = CreateSlider(0.01F, 3.0F, 0.01F, UHSGameUserSettings::GetHSGameUserSettings()->GetSensitivity(), TextSensitivity, 2, GameTabBox);
	SensitivitySlider->GetSlider()->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::SensitivityValueChanged);

	if (IsValid(FoVSlider)) {
		FoVSlider->RemoveFromParent();
	}

	FoVSlider = CreateSlider(60.0F, 120.0F, 1.0F, UHSGameUserSettings::GetHSGameUserSettings()->GetDefaultFoV(), TextFoV, 0, GameTabBox);
	FoVSlider->GetSlider()->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::DefaultFoVValueChanged);
}

void USettingsMenuWidget::SensitivityValueChanged(float NewVal)
{
	float Value = (int)(NewVal * 100.0F) / 100.0F;
	SensitivitySlider->SetValue(NewVal);
	UHSGameUserSettings::GetHSGameUserSettings()->SetSensitivity(Value);
}

void USettingsMenuWidget::DefaultFoVValueChanged(float NewVal)
{
	FoVSlider->SetValue(NewVal);
	UHSGameUserSettings::GetHSGameUserSettings()->SetDefaultFoV(NewVal);
}

void USettingsMenuWidget::InitGraphicsSettings()
{
	LowGraphicsPresetButton->GetButton()->OnClicked.AddDynamic(this, &USettingsMenuWidget::SetLowGraphicsQuality);
	MediumGraphicsPresetButton->GetButton()->OnClicked.AddDynamic(this, &USettingsMenuWidget::SetMediumGraphicsQuality);
	HighGraphicsPresetButton->GetButton()->OnClicked.AddDynamic(this, &USettingsMenuWidget::SetHighGraphicsQuality);
	UltraGraphicsPresetButton->GetButton()->OnClicked.AddDynamic(this, &USettingsMenuWidget::SetUltraGraphicsQuality);
	CustomGraphicsPresetButton->SetButtonText(TextCustom);
	LowGraphicsPresetButton->SetButtonText(TextLow);
	MediumGraphicsPresetButton->SetButtonText(TextMedium);
	HighGraphicsPresetButton->SetButtonText(TextHigh);
	UltraGraphicsPresetButton->SetButtonText(TextUltra);

	TArray<FText> SettingsName4Levels = { TextDisabled, TextLow, TextMedium, TextHigh, TextUltra };
	TArray<FText> SettingsNameNoDisbled4Levels = { TextLow, TextMedium, TextHigh, TextUltra };
	TArray<FText> SettingsNameNoDisbled5Levels = { TextLow, TextMedium, TextHigh, TextUltra, TextUnlimited };
	TArray<FText> SettingsName3Levels = { TextDisabled, TextLow, TextMedium, TextHigh };

	const FGraphicsSettings& CurrentQuality = UHSGameUserSettings::GetHSGameUserSettings()->GetQuality();
	SetGraphics(CurrentQuality);
	UpdateQualityIndex(CurrentQuality.QualityPresetIndex);

	InitResolutionSetting();

	if (IsValid(FullscreenOnOff))
		FullscreenOnOff->RemoveFromParent();

	FullscreenOnOff = CreateOnOff(CurrentQuality.bIsFullscreen, TextFullscreen, GraphicsTabBox);
	FullscreenOnOff->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

	if (IsValid(ScreenResolutionPercentSlider))
		ScreenResolutionPercentSlider->RemoveFromParent();

	ScreenResolutionPercentSlider = CreateSlider(50.0f, 100.0F, 1.0F, CurrentQuality.ResolutionScalePercent, TextRenderingQuality, 0, GraphicsTabBox);
	ScreenResolutionPercentSlider->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);
	
	if (IsValid(AAQualityComboBox))
		AAQualityComboBox->RemoveFromParent();

	AAQualityComboBox = CreateComboBox(SettingsName4Levels, CurrentQuality.AntiAaliasingQuality, TextAntiAliasing, GraphicsTabBox);
	AAQualityComboBox->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

#ifdef HS_WITH_DLSS
	if (IsValid(DLSSModeComboBox))
		DLSSModeComboBox->RemoveFromParent();

	DLSSModeComboBox = CreateComboBox(DLSSWrapper::GetAvailableDlssModes(), CurrentQuality.DLSSMode, TextDLSSMode, GraphicsTabBox);
	// Note: We use "ClampValue" == true here. It is workaround for the case is user change GPU or driver was changed,
	// so range of the slider values deceases, and we may need to clamp slider's current value
	DLSSModeComboBox->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);


	if (IsValid(DLSSSharpnessSlider))
		DLSSSharpnessSlider->RemoveFromParent();
	// DLSS Sharpness slider range: -1 - softening ... 1 - sharpening; step is 0.1 to allow user a simple way to set "0" but currently is not applied!
	// Note: optimal sharpness can be get from UDLSSLibrary::GetDLSSModeInformation(); see OutlineVFXComponent.cpp
	DLSSSharpnessSlider = CreateSlider(-1.0f, 1.0f, 0.1f, CurrentQuality.DLSSSharpness, TextDLSSSharpness, 1, GraphicsTabBox);
	DLSSSharpnessSlider->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);
#endif

	if (IsValid(ShadowsQualityComboBox))
		ShadowsQualityComboBox->RemoveFromParent();

	// According to UE's Scalability Reference document ShadowsQuality are "Low,Medium,High,Epic", so using SettingsNameNoDisbled4Levels here:
	ShadowsQualityComboBox = CreateComboBox(SettingsNameNoDisbled4Levels, CurrentQuality.ShadowsQuality, TextShadows, GraphicsTabBox);
	ShadowsQualityComboBox->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

	if (IsValid(ShadowsDrawDistanceMultSlider))
		ShadowsDrawDistanceMultSlider->RemoveFromParent();

	ShadowsDrawDistanceMultSlider = CreateSlider(0.0f, 2.0f, 0.01f, CurrentQuality.ShadowsDrawDistanceMult, TextShadowsDrawDistance, 2, GraphicsTabBox);
	ShadowsDrawDistanceMultSlider->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

	if (IsValid(PostProcessingQualityComboBox))
		PostProcessingQualityComboBox->RemoveFromParent();

	PostProcessingQualityComboBox = CreateComboBox(SettingsNameNoDisbled4Levels, CurrentQuality.PostProcessingQuality, TextPostProcessing, GraphicsTabBox);
	PostProcessingQualityComboBox->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

	if (IsValid(EffectsQualityComboBox))
		EffectsQualityComboBox->RemoveFromParent();

	EffectsQualityComboBox = CreateComboBox(SettingsNameNoDisbled4Levels, CurrentQuality.EffectsQuality, TextEffects, GraphicsTabBox);
	EffectsQualityComboBox->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

	if (IsValid(TexturesQualityComboBox))
		TexturesQualityComboBox->RemoveFromParent();

	TexturesQualityComboBox = CreateComboBox(SettingsNameNoDisbled4Levels, CurrentQuality.TexturesQuality, TextTextureResolution, GraphicsTabBox);
	TexturesQualityComboBox->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

	if (IsValid(SSLROnOff))
		SSLROnOff->RemoveFromParent();

	SSLROnOff = CreateOnOff(CurrentQuality.bSSLREnabled, TextSSLR, GraphicsTabBox);
	SSLROnOff->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

	if (IsValid(GlobalAOOnOff))
		GlobalAOOnOff->RemoveFromParent();

	GlobalAOOnOff = CreateOnOff(CurrentQuality.bGlobalAOEnabled, TextGlobalAO, GraphicsTabBox);
	GlobalAOOnOff->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

	if (IsValid(VSyncOnOff))
		VSyncOnOff->RemoveFromParent();

	VSyncOnOff = CreateOnOff(CurrentQuality.bVSyncEnabled, TextVSync, GraphicsTabBox);
	VSyncOnOff->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

	if (IsValid(AnisotropyQualityComboBox))
		AnisotropyQualityComboBox->RemoveFromParent();

	AnisotropyQualityComboBox = CreateComboBox({ TextDisabled , FText::FromString("X2") , FText::FromString("X4") , FText::FromString("X8") , FText::FromString("X16") }, CurrentQuality.AnisotropyQuality, TextAnisotropyLevel, GraphicsTabBox);
	AnisotropyQualityComboBox->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

	if (IsValid(MotionBlurComboBox))
		MotionBlurComboBox->RemoveFromParent();

	MotionBlurComboBox = CreateComboBox(SettingsName3Levels, CurrentQuality.MotionBlurQuality, TextMotionBlur, GraphicsTabBox);
	MotionBlurComboBox->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

	if (IsValid(AOComboBox))
		AOComboBox->RemoveFromParent();

	AOComboBox = CreateComboBox(SettingsName3Levels, CurrentQuality.AOQuality, TextAmbientOcclusion, GraphicsTabBox);
	AOComboBox->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

	if (IsValid(GammaSlider))
		GammaSlider->RemoveFromParent();

	GammaSlider = CreateSlider(1.5f, 3.0f, 0.01f, CurrentQuality.Gamma, TextGamma, 2, GraphicsTabBox);
	GammaSlider->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::GraphicsQualityChanged);

#ifdef HS_WITH_DLSS
	// Calling this here, though SetGraphics() is called above, because there DLSS widgets are not created yet
	UpdateDLSSWidgets(CurrentQuality);
#endif

	GraphicsQualityChangedEnabled = true;
}

void USettingsMenuWidget::InitResolutionSetting()
{
	const FGraphicsSettings& CurrentQuality = UHSGameUserSettings::GetHSGameUserSettings()->GetQuality();
	FIntPoint DeafultResolution = CurrentQuality.DefaultResolution;

	TArray<FText> ResolutionTexts;
	
	bool bHasDefaultResolution = false;
	for (int i = 0; i < Resolutions.Num(); i++)
	{
		if (Resolutions[i] == DeafultResolution)
		{
			bHasDefaultResolution = true;
			break;
		}
	}

	if (!bHasDefaultResolution)
	{
		Resolutions.Add(DeafultResolution);
		Resolutions.Sort([](const FIntPoint& A, const FIntPoint& B) {
			if (A.X == B.X)
				return A.Y > B.Y;
			else
				return A.X > B.X;
		});
	}

	auto DesktopResolution = UHSGameUserSettings::GetHSGameUserSettings()->GetDesktopResolution();
	auto Predicate = [&DesktopResolution](const FIntPoint Value)
	{
		return Value.X <= DesktopResolution.X && Value.Y <= DesktopResolution.Y;
	};

	Resolutions = Resolutions.FilterByPredicate(Predicate);

	int ResolutionIndex = 0;
	for (int i = 0; i < Resolutions.Num(); i++)
	{
		FString ResStr = FString::Format(TEXT("{0}x{1}"), { Resolutions[i].X, Resolutions[i].Y });
		ResolutionTexts.Add(FText::FromString(ResStr));

		if (Resolutions[i] == CurrentQuality.Resolution)
			ResolutionIndex = i;
	}

	if (IsValid(ResolutionComboBox))
		ResolutionComboBox->RemoveFromParent();

	ResolutionComboBox = CreateComboBox(ResolutionTexts, ResolutionIndex, TextResolution, GraphicsTabBox);
	ResolutionComboBox->OnValueChanged.AddDynamic(this, &USettingsMenuWidget::ResolutionChanged);
}

USettingsMenuSliderWidget* USettingsMenuWidget::CreateSlider(float InMinValue, float InMaxValue, float InStep, float InStartingValue, const FText& OptionName, int Precision, UScrollBox* Box)
{
	USettingsMenuSliderWidget* Slider = CreateWidget<USettingsMenuSliderWidget>(Box, SettingsMenuSliderWidgetBlueprint);
	Slider->InitSlider(InMinValue, InMaxValue, InStep, InStartingValue, OptionName, Precision);
	Cast<UScrollBoxSlot>(Box->AddChild(Slider))->SetPadding(FMargin(0.0F, 0.0F, 0.0F, 4.0F));

	return Slider;
}

USettingsMenuComboBoxWidget* USettingsMenuWidget::CreateComboBox(const TArray<FText>& Options, int InitialIndex, const FText& OptionName, UScrollBox* Box)
{
	USettingsMenuComboBoxWidget* ComboBox = CreateWidget<USettingsMenuComboBoxWidget>(Box, SettingsMenuComboBoxWidgetBlueprint);
	ComboBox->InitComboBox(Options, InitialIndex, OptionName);
	ComboBoxes.Add(ComboBox);
	Cast<UScrollBoxSlot>(Box->AddChild(ComboBox))->SetPadding(FMargin(0.0F, 0.0F, 0.0F, 4.0F));

	return ComboBox;
}

USettingsMenuOnOffWidget* USettingsMenuWidget::CreateOnOff(bool bInitialValue, const FText& OptionName, UScrollBox* Box)
{
	USettingsMenuOnOffWidget* OnOff = CreateWidget<USettingsMenuOnOffWidget>(Box, SettingsMenuOnOffWidgetBlueprint);
	OnOff->Init(bInitialValue, OptionName);
	Cast<UScrollBoxSlot>(Box->AddChild(OnOff))->SetPadding(FMargin(0.0F, 0.0F, 0.0F, 4.0F));

	return OnOff;
}

void USettingsMenuWidget::GraphicsQualityChanged()
{
	if (!GraphicsQualityChangedEnabled)		// Preventing GraphicsQualityChanged() execution during UpdateGraphicsWidgetsFromQuality()
		return;

	FGraphicsSettings OldQuality = UHSGameUserSettings::GetHSGameUserSettings()->GetQuality();
	FGraphicsSettings NewQuality = OldQuality;
	
	NewQuality.Gamma = GammaSlider->GetValue();
	NewQuality.bIsFullscreen = FullscreenOnOff->GetValue();
	NewQuality.ResolutionScalePercent = ScreenResolutionPercentSlider->GetValue();
	NewQuality.AntiAaliasingQuality = AAQualityComboBox->GetSelectedIndex();
	NewQuality.PostProcessingQuality = PostProcessingQualityComboBox->GetSelectedIndex();
	NewQuality.ShadowsQuality = ShadowsQualityComboBox->GetSelectedIndex();
	NewQuality.TexturesQuality = TexturesQualityComboBox->GetSelectedIndex();
	NewQuality.AnisotropyQuality = AnisotropyQualityComboBox->GetSelectedIndex();
	NewQuality.EffectsQuality = EffectsQualityComboBox->GetSelectedIndex();
	NewQuality.bSSLREnabled = SSLROnOff->GetValue();
	NewQuality.bGlobalAOEnabled = GlobalAOOnOff->GetValue();
	NewQuality.MotionBlurQuality = MotionBlurComboBox->GetSelectedIndex();
	NewQuality.AOQuality = AOComboBox->GetSelectedIndex();
	NewQuality.bVSyncEnabled = VSyncOnOff->GetValue();
	NewQuality.ShadowsDrawDistanceMult = ShadowsDrawDistanceMultSlider->GetValue();

#ifdef HS_WITH_DLSS
	NewQuality.DLSSMode = DLSSModeComboBox->GetSelectedIndex();
	NewQuality.DLSSSharpness = DLSSSharpnessSlider->GetValue();
#endif

	NewQuality.QualityPresetIndex = 0;

	UHSGameUserSettings::GetHSGameUserSettings()->SetQuality(NewQuality);
	UpdateGraphics(OldQuality, NewQuality);

	UpdateQualityIndex(0);
}

void USettingsMenuWidget::ResolutionChanged()
{
	FGraphicsSettings OldQuality = UHSGameUserSettings::GetHSGameUserSettings()->GetQuality();
	FGraphicsSettings NewQuality = OldQuality;
	NewQuality.Resolution = Resolutions[ResolutionComboBox->GetSelectedIndex()];
	NewQuality.bWasDefaultResolutionChanged = true;

	NewQuality.QualityPresetIndex = 0;

	UHSGameUserSettings::GetHSGameUserSettings()->SetQuality(NewQuality);
	UpdateGraphics(OldQuality, NewQuality);

	UpdateQualityIndex(0);
}

void USettingsMenuWidget::SetGraphics(const FGraphicsSettings& Quality)
{
	IConsoleVariable* CVarGamma = IConsoleManager::Get().FindConsoleVariable(TEXT("r.TonemapperGamma"));
	IConsoleVariable* CVarResolutionQuality = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	IConsoleVariable* CVarShadowQuality = IConsoleManager::Get().FindConsoleVariable(TEXT("sg.ShadowQuality")); 
	IConsoleVariable* CVarTextureAnisotropy = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MaxAnisotropy"));
	IConsoleVariable* CVarSkeletalMeshLODBias = IConsoleManager::Get().FindConsoleVariable(TEXT("r.SkeletalMeshLODBias"));
	IConsoleVariable* CVarVSync = IConsoleManager::Get().FindConsoleVariable(TEXT("r.VSync"));
	IConsoleVariable* CVarVolumetricFog = IConsoleManager::Get().FindConsoleVariable(TEXT("r.VolumetricFog"));

	CVarGamma->AsVariable()->Set(Quality.Gamma);
	CVarResolutionQuality->Set(Quality.ResolutionScalePercent);
	CVarShadowQuality->Set(Quality.ShadowsQuality); 
	CVarTextureAnisotropy->Set(FMath::Pow(2.0f, Quality.AnisotropyQuality));
	CVarParticleCullingScalability->Set(Quality.ParticleCullingScalability);
	CVarSkeletalMeshLODBias->Set(Quality.GetSkeletalMeshLODBias());
	CVarVSync->Set(Quality.bVSyncEnabled);

	SetupPostProcessingQuality(Quality.PostProcessingQuality);
	SetupTextureQuality(Quality.TexturesQuality); 
	SetupEffectsQuality(Quality.AnisotropyQuality);
	SetupSSLRQuality(Quality.bSSLREnabled); 
	SetupMotionBlurQuality(Quality.MotionBlurQuality);
	SetupAOQuality(Quality.AOQuality);
	SetupGlobalAOQuality(Quality.bGlobalAOEnabled);
	SetupDynamicWaterQuality(Quality.bDynamicWaterEnabled);
	SetupRefractionQuality(Quality.RefractionQuality);
	SetupShadowDrawDistance(Quality.ShadowsDrawDistanceMult);

	CVarVolumetricFog->Set(1);

#ifdef HS_WITH_DLSS
	ApplyDLSSSettings(Quality);
	UpdateDLSSWidgets(Quality);
#endif
}

void USettingsMenuWidget::UpdateGraphics(const FGraphicsSettings& OldQuality, const FGraphicsSettings& NewQuality)
{
	IConsoleVariable* CVarGamma = IConsoleManager::Get().FindConsoleVariable(TEXT("r.TonemapperGamma"));
	IConsoleVariable* CVarResolutionQuality = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	IConsoleVariable* CVarAntiAliasingQuality = IConsoleManager::Get().FindConsoleVariable(TEXT("r.PostProcessAAQuality"));
	IConsoleVariable* CVarShadowQuality = IConsoleManager::Get().FindConsoleVariable(TEXT("sg.ShadowQuality"));
	IConsoleVariable* CVarTextureAnisotropy = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MaxAnisotropy"));
	IConsoleVariable* CVarVSync = IConsoleManager::Get().FindConsoleVariable(TEXT("r.VSync"));

	AcceptSetting(CVarGamma, OldQuality.Gamma, NewQuality.Gamma);
	AcceptSetting(CVarResolutionQuality, OldQuality.ResolutionScalePercent, NewQuality.ResolutionScalePercent);
	AcceptSetting(CVarShadowQuality, OldQuality.ShadowsQuality, NewQuality.ShadowsQuality);

	if (OldQuality.Resolution != NewQuality.Resolution || OldQuality.bIsFullscreen != NewQuality.bIsFullscreen)
	{
		if (NewQuality.bIsFullscreen)
		{
			UHSGameUserSettings::GetHSGameUserSettings()->SetScreenResolution(UHSGameUserSettings::GetHSGameUserSettings()->GetDesktopResolution());
			UHSGameUserSettings::GetHSGameUserSettings()->SetFullscreenMode(EWindowMode::Fullscreen);
		}
		else
		{
			UHSGameUserSettings::GetHSGameUserSettings()->SetScreenResolution(NewQuality.Resolution);
			UHSGameUserSettings::GetHSGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
		}

		UHSGameUserSettings::GetHSGameUserSettings()->ApplyResolutionSettings(false);
	}

	if (OldQuality.AOQuality != NewQuality.AOQuality)
		SetupAOQuality(NewQuality.AOQuality);

	if (OldQuality.bGlobalAOEnabled != NewQuality.bGlobalAOEnabled)
		SetupGlobalAOQuality(NewQuality.bGlobalAOEnabled);

	if (OldQuality.bDynamicWaterEnabled != NewQuality.bDynamicWaterEnabled)
		SetupDynamicWaterQuality(NewQuality.bDynamicWaterEnabled);

	if (OldQuality.MotionBlurQuality != NewQuality.MotionBlurQuality)
		SetupMotionBlurQuality(NewQuality.MotionBlurQuality);

	if (OldQuality.PostProcessingQuality != NewQuality.PostProcessingQuality)
		SetupPostProcessingQuality(NewQuality.PostProcessingQuality);

	if (OldQuality.bSSLREnabled != NewQuality.bSSLREnabled)
		SetupSSLRQuality(NewQuality.bSSLREnabled);

	if (OldQuality.EffectsQuality != NewQuality.EffectsQuality)
		SetupEffectsQuality(NewQuality.AnisotropyQuality);

	if (OldQuality.AnisotropyQuality != NewQuality.AnisotropyQuality)
		CVarTextureAnisotropy->Set(FMath::Pow(2.0f, NewQuality.AnisotropyQuality));

	if (OldQuality.TexturesQuality != NewQuality.TexturesQuality)
		SetupTextureQuality(NewQuality.TexturesQuality);

	if (OldQuality.bVSyncEnabled != NewQuality.bVSyncEnabled)
		CVarVSync->Set(NewQuality.bVSyncEnabled);

	if (OldQuality.RefractionQuality != NewQuality.RefractionQuality)
		SetupRefractionQuality(NewQuality.RefractionQuality);

	if (OldQuality.ShadowsDrawDistanceMult != NewQuality.ShadowsDrawDistanceMult)
		SetupShadowDrawDistance(NewQuality.ShadowsDrawDistanceMult);

#ifdef HS_WITH_DLSS
	ApplyDLSSSettings(NewQuality, true, OldQuality);
	UpdateDLSSWidgets(NewQuality);
#endif
}

#ifdef HS_WITH_DLSS
void USettingsMenuWidget::ApplyDLSSSettings(const FGraphicsSettings& Quality, bool CheckChanging, const FGraphicsSettings& OldQuality)
{
	if (!CheckChanging || (OldQuality.DLSSMode != Quality.DLSSMode || OldQuality.DLSSSharpness != Quality.DLSSSharpness)) 
	{
		DLSSWrapper::ApplySettings({ DLSSModeUI(Quality.DLSSMode), Quality.DLSSSharpness });
	}
}

void USettingsMenuWidget::UpdateDLSSWidgets(const FGraphicsSettings& Quality) 
{
	bool DLSSSupported = DLSSWrapper::IsDLSSSupported();
	bool DLSSOn = DLSSSupported && Quality.DLSSMode != DLSSUI_Off;
	bool DLSSOnNotAuto = DLSSOn && Quality.DLSSMode != DLSSUI_Auto;

	// When DLSS is on, Screen resolution and AAQiality have effect only in Auto mode
	if (IsValid(ScreenResolutionPercentSlider))
		ScreenResolutionPercentSlider->SetIsEnabled(!DLSSOnNotAuto); 
	if (IsValid(AAQualityComboBox))
		AAQualityComboBox->SetIsEnabled(!DLSSOnNotAuto);	

	if (IsValid(DLSSModeComboBox))
		DLSSModeComboBox->SetIsEnabled(DLSSSupported);
	if (IsValid(DLSSSharpnessSlider))
		DLSSSharpnessSlider->SetIsEnabled(DLSSOnNotAuto);
}
#endif

void USettingsMenuWidget::SetLowGraphicsQuality()
{
	FGraphicsSettings OldQuality = UHSGameUserSettings::GetHSGameUserSettings()->GetQuality();
	FGraphicsSettings NewQuality = OldQuality;
	NewQuality.QualityPresetIndex = 1;

	NewQuality.ResolutionScalePercent = 100.0F;
	NewQuality.AntiAaliasingQuality = 1;
#ifdef HS_WITH_DLSS
	NewQuality.DLSSMode = DLSSUI_Auto;
	NewQuality.DLSSSharpness = 0;
#endif
	NewQuality.SkeletalMeshQuality = 0;
	NewQuality.DecoratorQuality = 0;
	NewQuality.ShadowsQuality = 0;
	NewQuality.PostProcessingQuality = 0;
	NewQuality.EffectsQuality = 1;
	NewQuality.TexturesQuality = 1;
	NewQuality.bSSLREnabled = false;
	NewQuality.bGlobalAOEnabled = false;
	NewQuality.bDynamicWaterEnabled = false;
	NewQuality.AnisotropyQuality = 1;
	NewQuality.MotionBlurQuality = 0;
	NewQuality.AOQuality = 0;
	NewQuality.ParticleCullingScalability = 0;
	NewQuality.GrassDecoratorQuality = 1;
	NewQuality.RefractionQuality = 1;
	NewQuality.ShadowsDrawDistanceMult = 0.6;

	UHSGameUserSettings::GetHSGameUserSettings()->SetQuality(NewQuality);
	UpdateGraphics(OldQuality, NewQuality);
	UpdateGraphicsWidgetsFromQuality(NewQuality);

	UpdateQualityIndex(1);
}

void USettingsMenuWidget::SetMediumGraphicsQuality()
{
	FGraphicsSettings OldQuality = UHSGameUserSettings::GetHSGameUserSettings()->GetQuality();
	FGraphicsSettings NewQuality = OldQuality;
	NewQuality.QualityPresetIndex = 2;

	NewQuality.ResolutionScalePercent = 100.0F;
	NewQuality.AntiAaliasingQuality = 2;
#ifdef HS_WITH_DLSS
	NewQuality.DLSSMode = DLSSUI_Auto;
	NewQuality.DLSSSharpness = 0;
#endif
	NewQuality.SkeletalMeshQuality = 2;
	NewQuality.DecoratorQuality = 1;
	NewQuality.ShadowsQuality = 1;
	NewQuality.PostProcessingQuality = 1;
	NewQuality.EffectsQuality = 1;
	NewQuality.TexturesQuality = 1;
	NewQuality.bSSLREnabled = false;
	NewQuality.bGlobalAOEnabled = true;
	NewQuality.bDynamicWaterEnabled = true;
	NewQuality.AnisotropyQuality = 3;
	NewQuality.MotionBlurQuality = 2;
	NewQuality.AOQuality = 2;
	NewQuality.ParticleCullingScalability = 1;
	NewQuality.GrassDecoratorQuality = 2;
	NewQuality.RefractionQuality = 1;
	NewQuality.ShadowsDrawDistanceMult = 0.7;

	UHSGameUserSettings::GetHSGameUserSettings()->SetQuality(NewQuality);
	UpdateGraphics(OldQuality, NewQuality);
	UpdateGraphicsWidgetsFromQuality(NewQuality);
}

void USettingsMenuWidget::SetHighGraphicsQuality()
{
	FGraphicsSettings OldQuality = UHSGameUserSettings::GetHSGameUserSettings()->GetQuality();
	FGraphicsSettings NewQuality = OldQuality;
	NewQuality.QualityPresetIndex = 3;

	NewQuality.ResolutionScalePercent = 100.0F;
	NewQuality.AntiAaliasingQuality = 3;
#ifdef HS_WITH_DLSS
	NewQuality.DLSSMode = DLSSUI_Auto;
	NewQuality.DLSSSharpness = 0;
#endif
	NewQuality.SkeletalMeshQuality = 2;
	NewQuality.DecoratorQuality = 2;
	NewQuality.ShadowsQuality = 2;
	NewQuality.PostProcessingQuality = 2;
	NewQuality.EffectsQuality = 2;
	NewQuality.TexturesQuality = 2;
	NewQuality.bSSLREnabled = true;
	NewQuality.bGlobalAOEnabled = true;
	NewQuality.bDynamicWaterEnabled = true;
	NewQuality.AnisotropyQuality = 4;
	NewQuality.MotionBlurQuality = 3;
	NewQuality.AOQuality = 3;
	NewQuality.ParticleCullingScalability = 3;
	NewQuality.GrassDecoratorQuality = 3;
	NewQuality.RefractionQuality = 2;
	NewQuality.ShadowsDrawDistanceMult = 0.8;

	UHSGameUserSettings::GetHSGameUserSettings()->SetQuality(NewQuality);
	UpdateGraphics(OldQuality, NewQuality);
	UpdateGraphicsWidgetsFromQuality(NewQuality);
}

void USettingsMenuWidget::SetUltraGraphicsQuality()
{
	FGraphicsSettings OldQuality = UHSGameUserSettings::GetHSGameUserSettings()->GetQuality();
	FGraphicsSettings NewQuality = OldQuality;
	NewQuality.QualityPresetIndex = 4;

	NewQuality.ResolutionScalePercent = 100.0F;
	NewQuality.AntiAaliasingQuality = 4;
#ifdef HS_WITH_DLSS
	NewQuality.DLSSMode = DLSSUI_Auto;
	NewQuality.DLSSSharpness = 0;
#endif
	NewQuality.SkeletalMeshQuality = 3;
	NewQuality.DecoratorQuality = 3;
	NewQuality.ShadowsQuality = 3;
	NewQuality.PostProcessingQuality = 3;
	NewQuality.EffectsQuality = 3;
	NewQuality.TexturesQuality = 3;
	NewQuality.bSSLREnabled = true;
	NewQuality.bGlobalAOEnabled = true;
	NewQuality.bDynamicWaterEnabled = true;
	NewQuality.AnisotropyQuality = 4;
	NewQuality.MotionBlurQuality = 3;
	NewQuality.AOQuality = 3;
	NewQuality.ParticleCullingScalability = 4;
	NewQuality.GrassDecoratorQuality = 3;
	NewQuality.RefractionQuality = 2;
	NewQuality.ShadowsDrawDistanceMult = 1.0;

	UHSGameUserSettings::GetHSGameUserSettings()->SetQuality(NewQuality);
	UpdateGraphics(OldQuality, NewQuality);
	UpdateGraphicsWidgetsFromQuality(NewQuality);
}

void USettingsMenuWidget::UpdateQualityIndex(int Index)
{
	CustomGraphicsPresetButton->SetActiveState(Index == 0);
	LowGraphicsPresetButton->SetActiveState(Index == 1);
	MediumGraphicsPresetButton->SetActiveState(Index == 2);
	HighGraphicsPresetButton->SetActiveState(Index == 3);
	UltraGraphicsPresetButton->SetActiveState(Index == 4);
}

void USettingsMenuWidget::RestoreDefaultsGraphics()
{
	FGraphicsSettings DefaultGraphics;
	DefaultGraphics.Resolution = UHSGameUserSettings::GetHSGameUserSettings()->GetQuality().DefaultResolution;

	UpdateGraphics(UHSGameUserSettings::GetHSGameUserSettings()->GetQuality(), DefaultGraphics);
	UpdateGraphicsWidgetsFromQuality(DefaultGraphics);

	UHSGameUserSettings::GetHSGameUserSettings()->SetQuality(DefaultGraphics);
}

void USettingsMenuWidget::RestoreDefaultsControls()
{
	UHSGameUserSettings::GetHSGameUserSettings()->ClearBindingUpdates();

	for (int i = 0; i < BindingEntries.Num(); i++)
	{
		BindingEntries[i]->RestoreDefaults();
	}
}

void USettingsMenuWidget::RestoreDefaultsInterface()
{

}

void USettingsMenuWidget::RestoreDefaultsGame()
{
	SensitivityValueChanged(DefaultSettings::DefaultSensitivity);
	DefaultFoVValueChanged(DefaultSettings::DefaultFoV);
}

void USettingsMenuWidget::RestoreDefaultsAll()
{
	RestoreDefaultsGame();
	RestoreDefaultsInterface();
	RestoreDefaultsControls();
	RestoreDefaultsGraphics();
}

void USettingsMenuWidget::RestoreDefaults()
{
	auto ActiveWidget = TabSwitcher->GetActiveWidget();
	auto ActiveWidgetIndex = TabSwitcher->GetActiveWidgetIndex();

	if (ActiveWidget == GameTabBox)
	{
		RestoreDefaultsGame();
	}
	else if (ActiveWidget == InterfaceTabBox)
	{
		RestoreDefaultsInterface();
	}
	else if (ActiveWidget == ScrollBoxKeyBindings)
	{
		RestoreDefaultsControls();
	}
	else if (ActiveWidgetIndex == 3)
	{
		RestoreDefaultsGraphics();
	}
	else
	{
		RestoreDefaultsAll();
	}
}

void USettingsMenuWidget::UpdateGraphicsWidgetsFromQuality(const FGraphicsSettings& GraphicsSettings)
{
	GraphicsQualityChangedEnabled = false;		// Disable GraphicsQualityChanged() execution to prevent executing callbacks from controls

	GammaSlider->SetValue(GraphicsSettings.Gamma);
	ScreenResolutionPercentSlider->SetValue(GraphicsSettings.ResolutionScalePercent);
	ShadowsDrawDistanceMultSlider->SetValue(GraphicsSettings.ShadowsDrawDistanceMult);
	AAQualityComboBox->SetSelectedIndex(GraphicsSettings.AntiAaliasingQuality);

#ifdef HS_WITH_DLSS
	DLSSModeComboBox->SetSelectedIndex(DLSSWrapper::ModeToListIndex(UDLSSMode(GraphicsSettings.DLSSMode)));
	DLSSSharpnessSlider->SetValue(GraphicsSettings.DLSSSharpness);
#endif

	PostProcessingQualityComboBox->SetSelectedIndex(GraphicsSettings.PostProcessingQuality);
	ShadowsQualityComboBox->SetSelectedIndex(GraphicsSettings.ShadowsQuality);
	TexturesQualityComboBox->SetSelectedIndex(GraphicsSettings.TexturesQuality);
	AnisotropyQualityComboBox->SetSelectedIndex(GraphicsSettings.AnisotropyQuality);
	EffectsQualityComboBox->SetSelectedIndex(GraphicsSettings.EffectsQuality);
	SSLROnOff->SetValue(GraphicsSettings.bSSLREnabled);
	GlobalAOOnOff->SetValue(GraphicsSettings.bGlobalAOEnabled);
	MotionBlurComboBox->SetSelectedIndex(GraphicsSettings.MotionBlurQuality);
	AOComboBox->SetSelectedIndex(GraphicsSettings.AOQuality);

	UpdateQualityIndex(GraphicsSettings.QualityPresetIndex);

	GraphicsQualityChangedEnabled = true;		// Enable GraphicsQualityChanged() execution back
}

void USettingsMenuWidget::BackButtonClicked()
{
	for (int i = ScrollBoxKeyBindings->GetChildrenCount() - 1; i >= 0; i--)
	{
		ScrollBoxKeyBindings->RemoveChildAt(i);
	}

	InitControlSettings();
}
