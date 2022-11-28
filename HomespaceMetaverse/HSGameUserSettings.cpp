#include "HSGameUserSettings.h"
#include "Widgets/SettingsMenu/SettingsConsts.h"
#include "KeyBindingUtil.h"

#include "Engine/Engine.h"
 
struct FTextureQualitySettings
{
	float MipBias = 0.0f;
	float PoolSize = 1000.0f;
};
 
struct FEffectsQualitySettings
{
	int TranslucencyLightingVolumeDim = 24;
	int RefractionQuality = 0;
	int SceneColorFormat = 3;
	int DetailMode = 0;
	int TranslucencyVolumeBlur = 0;
	int MaterialQualityLevel = 0;
};

struct FPostProcessingQuality
{
	int DepthOfFieldQuality = 0;
	int RenderTargetPoolMin = 300;
	int LensFlareQuality = 0;
	int SceneColorFringeQuality = 0;
	int EyeAdaptationQuality = 0;
	int BloomQuality = 4;
	int FastBlurThreshold = 0;
	int UpscaleQuality = 1;
	int TonemapperGrainQuantization = 1;
};

struct FMotionBlurQuality
{
	int BlurGBuffer = 0;
	int MotionBlurQuality = 0;
};

struct FAOQuality
{
	int AmbientOcclusionLevels = 0;
	float AmbientOcclusionRadiusScale = 1.7f;
};

void SetupTextureQuality(int QualityIndex)
{
	static const TArray<FTextureQualitySettings> QualityLevels = {
		{2.5f, 200.0f},
		{2.0f, 500.0f},
		{1.0f, 700.0f},
		{0.0f, 1000.0f}
	};

	if (QualityIndex < 0 || QualityIndex >= QualityLevels.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("SetupTextureQuality(): QualityIndex (%i) out of range, must be in 0...%i"), QualityIndex, QualityLevels.Num() - 1);
		QualityIndex = FMath::Clamp(QualityIndex, 0, QualityLevels.Num() - 1);
	} 

	ConsoleSetHelper(TEXT("r.Streaming.MipBias"), QualityLevels[QualityIndex].MipBias);
	ConsoleSetHelper(TEXT("r.Streaming.PoolSize"), QualityLevels[QualityIndex].PoolSize);
}

void SetupEffectsQuality(int QualityIndex)
{
	static const TArray<FEffectsQualitySettings> QualityLevels = {
		 {24, 1, 3, 0, 0, 0},
		 {32, 1, 3, 1, 0, 1},
		 {48, 2, 3, 1, 1, 1},
		 {64, 2, 4, 1, 1, 1},
	};

	if (QualityIndex < 0 || QualityIndex >= QualityLevels.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("SetupEffectsQuality(): QualityIndex (%i) out of range, must be in 0...%i"), QualityIndex, QualityLevels.Num() - 1);
		QualityIndex = FMath::Clamp(QualityIndex, 0, QualityLevels.Num() - 1);
	}

	ConsoleSetHelper(TEXT("r.TranslucencyLightingVolumeDim"), QualityLevels[QualityIndex].TranslucencyLightingVolumeDim);
	ConsoleSetHelper(TEXT("r.RefractionQuality"), QualityLevels[QualityIndex].RefractionQuality);
	ConsoleSetHelper(TEXT("r.SceneColorFormat"), QualityLevels[QualityIndex].SceneColorFormat);
	ConsoleSetHelper(TEXT("r.DetailMode"), QualityLevels[QualityIndex].DetailMode);
	ConsoleSetHelper(TEXT("r.TranslucencyVolumeBlur"), QualityLevels[QualityIndex].TranslucencyVolumeBlur);
	ConsoleSetHelper(TEXT("r.MaterialQualityLevel"), QualityLevels[QualityIndex].MaterialQualityLevel); 
}

void SetupSSLRQuality(bool bEnabled)
{
	ConsoleSetHelper(TEXT("r.SSR.Quality"), bEnabled);
}

void SetupGlobalAOQuality(bool bEnabled)
{
	ConsoleSetHelper(TEXT(VOXEL_GLOBAL_AO_STR), bEnabled);
}

void SetupDynamicWaterQuality(bool bEnabled)
{
	ConsoleSetHelper(TEXT(DYNAMIC_WATER_STR), bEnabled);
}

void SetupPostProcessingQuality(int QualityIndex)
{
	static const TArray<FPostProcessingQuality> QualityLevels = {
		{0, 300, 0, 0, 0, 4, 0, 1, 1 },
		{1, 350, 0, 0, 0, 4, 2, 2, 1 },
		{2, 400, 2, 1, 2, 5, 5, 2, 1 },
		{2, 400, 2, 1, 2, 5, 7, 3, 1 }
	};

	if (QualityIndex < 0 || QualityIndex >= QualityLevels.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("SetupPostProcessingQuality(): QualityIndex (%i) out of range, must be in 0...%i"), QualityIndex, QualityLevels.Num() - 1);
		QualityIndex = FMath::Clamp(QualityIndex, 0, QualityLevels.Num() - 1);
	}

	ConsoleSetHelper(TEXT("r.DepthOfFieldQuality"), QualityLevels[QualityIndex].DepthOfFieldQuality);
	ConsoleSetHelper(TEXT("r.RenderTargetPoolMin"), QualityLevels[QualityIndex].RenderTargetPoolMin);
	ConsoleSetHelper(TEXT("r.LensFlareQuality"), QualityLevels[QualityIndex].LensFlareQuality);
	ConsoleSetHelper(TEXT("r.SceneColorFringeQuality"), QualityLevels[QualityIndex].SceneColorFringeQuality);
	ConsoleSetHelper(TEXT("r.EyeAdaptationQuality"), QualityLevels[QualityIndex].EyeAdaptationQuality);
	ConsoleSetHelper(TEXT("r.BloomQuality"), QualityLevels[QualityIndex].BloomQuality);
	ConsoleSetHelper(TEXT("r.FastBlurThreshold"), QualityLevels[QualityIndex].FastBlurThreshold);
	ConsoleSetHelper(TEXT("r.Upscale.Quality"), QualityLevels[QualityIndex].UpscaleQuality);
	ConsoleSetHelper(TEXT("r.Tonemapper.GrainQuantization"), QualityLevels[QualityIndex].TonemapperGrainQuantization); 
}

void SetupMotionBlurQuality(int QualityIndex)
{
	static const TArray<FMotionBlurQuality> QualityLevels = {
		{0, 0},
		{0, 3},
		{-1, 3},
		{-1, 4},
	};

	if (QualityIndex < 0 || QualityIndex >= QualityLevels.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("SetupMotionBlurQuality(): QualityIndex (%i) out of range, must be in 0...%i"), QualityIndex, QualityLevels.Num() - 1);
		QualityIndex = FMath::Clamp(QualityIndex, 0, QualityLevels.Num() - 1);
	}

	ConsoleSetHelper(TEXT("r.MotionBlurQuality"), QualityLevels[QualityIndex].MotionBlurQuality);
//	ConsoleSetHelper(TEXT("r.BlurGBuffer"), QualityLevels[QualityIndex].BlurGBuffer);
}

void SetupRefractionQuality(int QualityIndex)
{
	static const TArray<int> QualityLevels = {
		0,
		1,
		2
	};

	if (QualityIndex < 0 || QualityIndex >= QualityLevels.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("SetupRefractionQuality(): QualityIndex (%i) out of range, must be in 0...%i"), QualityIndex, QualityLevels.Num() - 1);
		QualityIndex = FMath::Clamp(QualityIndex, 0, QualityLevels.Num() - 1);
	}

	ConsoleSetHelper(TEXT("r.RefractionQuality"), QualityLevels[QualityIndex]);
}

void SetupShadowDrawDistance(float Quality)
{
	float MaxDrawDistance = 2.0f;

	if (Quality < 0 || Quality >= MaxDrawDistance)
	{
		UE_LOG(LogTemp, Log, TEXT("SetupShadowDrawDistance(): Quality (%f) out of range, must be in 0...%f"), Quality, MaxDrawDistance);
		Quality = FMath::Clamp(Quality, 0.0f, MaxDrawDistance);
	}

	ConsoleSetHelper(TEXT("r.Shadow.DistanceScale"), Quality);
}

void SetupAOQuality(int QualityIndex)
{
	static const TArray<FAOQuality> QualityLevels = {
		{0, 1.7f},
		{1, 1.7f},
		{2, 1.5f},
		{3, 1.0f},
	};

	if (QualityIndex < 0 || QualityIndex >= QualityLevels.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("SetupAOQuality(): QualityIndex (%i) out of range, must be in 0...%i"), QualityIndex, QualityLevels.Num() - 1);
		QualityIndex = FMath::Clamp(QualityIndex, 0, QualityLevels.Num() - 1);
	}

	ConsoleSetHelper(TEXT("r.AmbientOcclusionLevels"), QualityLevels[QualityIndex].AmbientOcclusionLevels);
	ConsoleSetHelper(TEXT("r.AmbientOcclusionRadiusScale"), QualityLevels[QualityIndex].AmbientOcclusionRadiusScale);
}

UHSGameUserSettings::UHSGameUserSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Sensitivity = 1;
	DefaultFoV = 90.0F; 
	FoVUW = false;
}

UHSGameUserSettings* UHSGameUserSettings::GetHSGameUserSettings()
{
	if (GEngine)
		return Cast<UHSGameUserSettings>(GEngine->GetGameUserSettings());
	else
		return nullptr;
}

const FString& UHSGameUserSettings::GetPlayerName() const
{
	return PlayerName;
}

void UHSGameUserSettings::SetPlayerName(FString NewPlayerName)
{
	PlayerName = NewPlayerName;
	SaveSettings();
}

float UHSGameUserSettings::GetSensitivity() const
{
	return Sensitivity;
}

void UHSGameUserSettings::SetSensitivity(float InSensitivity)
{
	Sensitivity = InSensitivity;
	SaveSettings();
}

float UHSGameUserSettings::GetDefaultFoV() const
{
	return DefaultFoV;
}

void UHSGameUserSettings::SetDefaultFoV(float InFoV)
{
	DefaultFoV = InFoV;
	SaveSettings();
}

bool UHSGameUserSettings::GetFoVUW() const
{
	return FoVUW;
}

void UHSGameUserSettings::SetFoVUW(bool InFoVUW)
{
	FoVUW = InFoVUW;
}

void UHSGameUserSettings::SetAllowBackfill(bool InAllowBackfill)
{
	AllowBackfill = InAllowBackfill;
}

const FGraphicsSettings& UHSGameUserSettings::GetQuality() const
{
	return Quality;
}

void UHSGameUserSettings::SetQuality(const FGraphicsSettings& InQuality)
{
	Quality = InQuality;
	SaveSettings();
}

void UHSGameUserSettings::SetLocale(ELocale InLocale)
{
	Locale = InLocale;
	SaveSettings();
}

ELocale UHSGameUserSettings::GetLocale() const
{
	return Locale;
}

bool UHSGameUserSettings::GetOverrideLocale() const
{
	return bOverrideLocale;
}

void UHSGameUserSettings::SetOverrideLocale(bool InOverrideLocale)
{
	bOverrideLocale = InOverrideLocale;
	SaveSettings();
}

void UHSGameUserSettings::SetMute(bool bEnable)
{
	FootstepSounds = !bEnable;
	ImpactSounds = !bEnable;
	WeaponSounds = !bEnable;
	UnitsSounds = !bEnable;
	VoxelsSounds = !bEnable;
	VoiceOverSounds = !bEnable;
	FeedbackSounds = !bEnable;
	AnnouncementSounds = !bEnable;
	Music = !bEnable;
}

void UHSGameUserSettings::SetBindingUpdate(bool IsAxis, int Index, float Scale, const FString& Name, const FKey& NewKey, const FKey& OldKey)
{
	if (NewKey == OldKey)
	{
		return;
	}

	FBindingUpdate Update(IsAxis, Index, Scale, Name);

	int ArrayIndex = BindingUpdates.Find(Update);
	if (ArrayIndex != INDEX_NONE)
	{
		if (NewKey == BindingUpdates[ArrayIndex].DefaultKey)
		{
			BindingUpdates.RemoveAt(ArrayIndex);
		}
		else
		{
			BindingUpdates[ArrayIndex].Key = NewKey;
		}
	}
	else
	{
		Update.Key = NewKey;
		Update.DefaultKey = OldKey;
		BindingUpdates.Add(Update);
	}

	SaveSettings();
}

FBindingUpdate UHSGameUserSettings::FindBindingUpdate(bool IsAxis, int Index, float Scale, const FString& Name)
{
	FBindingUpdate Update(IsAxis, Index, Scale, Name);
	int ArrayIndex = BindingUpdates.Find(Update);
	if (ArrayIndex != INDEX_NONE)
	{
		return BindingUpdates[ArrayIndex];
	}

	return FBindingUpdate();
}

void UHSGameUserSettings::ClearBindingUpdates()
{
	BindingUpdates.Empty(0);
	SaveSettings();
}
