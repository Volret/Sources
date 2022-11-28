#pragma once

#include "CoreMinimal.h" 
#include "GameFramework/GameUserSettings.h"

#include "HSGameUserSettings.generated.h"

#define VOXEL_GLOBAL_AO_STR "r.GlobalAOQuality"
#define DYNAMIC_WATER_STR "r.DynamicWaterQuality"
#define PARTICLE_CULLING_SCALABILITY_STR "r.ParticleCullingScalability"

static TAutoConsoleVariable<bool> CVarGlobalAOQuality(
	TEXT(VOXEL_GLOBAL_AO_STR),
	false,
	TEXT("Defines the global AO enabled or disabled\n")
	TEXT("0: Disabled\n")
	TEXT("1: Enabled\n"),
	ECVF_Scalability | ECVF_RenderThreadSafe
);

static TAutoConsoleVariable<bool> CVarDynamicWaterQuality(
	TEXT(DYNAMIC_WATER_STR),
	false,
	TEXT("Defines the dynamic water enabled or disabled\n")
	TEXT("0: Disabled\n")
	TEXT("1: Enabled\n"),
	ECVF_Scalability | ECVF_RenderThreadSafe
);

static TAutoConsoleVariable<int> CVarParticleCullingScalability(
	TEXT(PARTICLE_CULLING_SCALABILITY_STR),
	3,
	TEXT("Defines the particles culling scalability\n")
	TEXT("0: low\n")
	TEXT("1: normal\n")
	TEXT("2: high\n")
	TEXT("3: ultra\n")
	TEXT("4: unlimited\n"),
	ECVF_Scalability | ECVF_RenderThreadSafe
);

struct ParticleCullingScalabilityData
{
	float CullingDistanceBase = 1000.0f;
	float BoundFactor = 3.0f;
	float ImportantCullingDistanceFactor = 1.0f;
	float InsignificantCullingDistanceFactor = 1.0f;
	int MaxEmittors = 100; 
	int MinCullingEmitters = 50;
	int EmmitorOverloadCount = 80;
};
 
static const TArray<ParticleCullingScalabilityData> ParticleCullingScalabilityLevels =
{
	{ 800.0f, 10.0f, 20.0f, 0.55f, 40, 10, 20},
	{ 1000.0f, 12.0, 20.0f, 0.55f, 60, 15, 40},
	{ 1200.0f, 14.0f, 25.0f, 0.6f, 80, 20, 60},
	{ 1500.0f, 16.0, 25.0f, 0.6f, 100, 25, 80},
};

template<typename Type> void ConsoleSetHelper(const TCHAR* Name, const Type& Value)
{
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(Name);

	if (CVar)
	{
		CVar->Set(Value);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ConsoleSetHelper(): Fail to find varible \"%s\""), Name);
	}
}

void SetupGlobalAOQuality(bool bEnabled);
void SetupTextureQuality(int QualityIndex);
void SetupEffectsQuality(int QualityIndex);
void SetupSSLRQuality(bool bEnabled);
void SetupPostProcessingQuality(int QualityIndex);
void SetupMotionBlurQuality(int QualityIndex);
void SetupAOQuality(int QualityIndex);
void SetupRefractionQuality(int QualityIndex);
void SetupShadowDrawDistance(float Quality);
void SetupDynamicWaterQuality(bool bEnabled);

USTRUCT(BlueprintType)
struct FGraphicsSettings
{
	GENERATED_BODY()

	UPROPERTY() int RefractionQuality = 1;
	UPROPERTY() int QualityPresetIndex = 4;
	UPROPERTY() FIntPoint Resolution;
	UPROPERTY() bool bIsFullscreen = true;
	UPROPERTY() FIntPoint DefaultResolution;
	UPROPERTY() bool bWasDefaultResolutionChanged = false;
	UPROPERTY() float ResolutionScalePercent = 100.0f;
	UPROPERTY() bool bVSyncEnabled = true;
	UPROPERTY() int32 AntiAaliasingQuality = 4;
	UPROPERTY() int32 PostProcessingQuality = 3;
	UPROPERTY() int32 ShadowsQuality = 3;
	UPROPERTY() float ShadowsDrawDistanceMult = 1.0f;
	UPROPERTY() int32 TexturesQuality = 3;
	UPROPERTY() int32 AnisotropyQuality = 4;
	UPROPERTY() int32 EffectsQuality = 3;
	UPROPERTY()	float Gamma = 2.2f;
	UPROPERTY()	bool bSSLREnabled = true;
	UPROPERTY()	bool bGlobalAOEnabled = true;
	UPROPERTY()	int32 MotionBlurQuality = 3;
	UPROPERTY()	int32 AOQuality = 3;
	UPROPERTY()	int32 GrassDecoratorQuality = 3;
	UPROPERTY()	int32 ParticleCullingScalability = 4;
	UPROPERTY()	bool bDynamicWaterEnabled = true;
	// DLSS
	UPROPERTY()	int32 DLSSMode = 0;	// Off
	UPROPERTY()	float DLSSSharpness = 0;	// -1 - softening, 1 - sharpening 

	static constexpr int32 MaxSkeletalMeshLODBias = 3;
	static constexpr int32 MaxDecoratorLODBias = 3;

	UPROPERTY()	int32 SkeletalMeshQuality = MaxSkeletalMeshLODBias;
	UPROPERTY()	int32 DecoratorQuality = MaxDecoratorLODBias;

	//
	UPROPERTY()	bool bHitAnimationEnabled = true;
	UPROPERTY()	bool bWeaponPhysicsEnabled = true;

	int32_t GetSkeletalMeshLODBias() const
	{
		return MaxSkeletalMeshLODBias - SkeletalMeshQuality;
	}

	int32_t GetDecoratorLODBias() const
	{
		return MaxDecoratorLODBias - DecoratorQuality;
	}
};

USTRUCT()
struct FBindingUpdate
{
	GENERATED_BODY()

	FBindingUpdate() : bIsAxis(false), Scale(0.0F), Index(-1) {}
	FBindingUpdate(bool InIsAxis, int InIndex, float InScale, const FString& InName) : bIsAxis(InIsAxis), Scale(InScale), Index(InIndex), Name(InName) {}

	UPROPERTY() bool bIsAxis;
	UPROPERTY() float Scale;
	UPROPERTY() int Index;
	UPROPERTY() FString Name;
	UPROPERTY() FKey Key;
	UPROPERTY() FKey DefaultKey;

	FORCEINLINE bool operator==(const FBindingUpdate& Other) const
	{
		return bIsAxis == Other.bIsAxis && FMath::IsNearlyEqual(Scale, Other.Scale) && Index == Other.Index && Name == Other.Name;
	};
};

UCLASS(BlueprintType)
class HOMESPACEMETAVERSE_API UHSGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:	
	UHSGameUserSettings(const FObjectInitializer& ObjectInitializer);

	/** Returns the game local machine settings (resolution, windowing mode, scalability settings, etc...) */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	static UHSGameUserSettings* GetHSGameUserSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	const FString& GetPlayerName() const;
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetPlayerName(FString NewPlayerName);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	float GetSensitivity() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetSensitivity(float InSensitivity);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	float GetDefaultFoV() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetDefaultFoV(float InFoV);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	bool GetFoVUW() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetFoVUW(bool InFoVUW);

	bool GetAllowBackfill() const { return AllowBackfill; }

	void SetAllowBackfill(bool InAllowBackfill);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	const FGraphicsSettings& GetQuality() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetLocale(ELocale InLocale);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	ELocale GetLocale() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	bool GetOverrideLocale() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetOverrideLocale(bool InOverrideLocale);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetQuality(const FGraphicsSettings& InQuality);
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetMute(bool bEnable);


	UPROPERTY(Config)
	float OverallAudioBusVolume = 1.f;

	UPROPERTY(Config)
	float MusicAudioBusVolume = 1.f;

	UPROPERTY(Config)
	float SFXAudioBusVolume = 1.f;
	
	bool FootstepSounds = true;
	bool ImpactSounds = true;
	bool WeaponSounds = true;
	bool UnitsSounds = true;
	bool VoxelsSounds = true;
	bool FeedbackSounds = true;
	bool VoiceOverSounds = true;
	bool AnnouncementSounds = true;
	bool Music = true;
	bool GuaranteedVOs = false;

	bool SpawnVFX = true;

	bool bWasKickedForAFK = false;

	//Match result
	bool bMatchEndNeedToShowInMainMenu = false;

	//Bindings
	void SetBindingUpdate(bool IsAxis, int Index, float Scale, const FString& Name, const FKey& NewKey, const FKey& OldKey);
	FBindingUpdate FindBindingUpdate(bool IsAxis, int Index, float Scale, const FString& Name);
	const TArray<FBindingUpdate>& GetBindingUpdates() { return BindingUpdates; }
	void ClearBindingUpdates();
	
protected:
	UPROPERTY(config)
	FString PlayerName;

	UPROPERTY(config)
	float Sensitivity;

	UPROPERTY(config)
	float DefaultFoV;

	UPROPERTY(config)
	bool FoVUW;

	UPROPERTY(config)
	bool AllowBackfill;
	 
	UPROPERTY(config)
	FGraphicsSettings Quality;

	UPROPERTY(config)
	TArray<FBindingUpdate> BindingUpdates;

	UPROPERTY(config)
	ELocale Locale;

	UPROPERTY(config)
	bool bOverrideLocale = false;

	UPROPERTY(config)
	FString CustomStartMapId = "";

	UPROPERTY(config)
	bool bIsLicenceAgreementAccepted = false;

	UPROPERTY(config)
	bool bIsTutorialPageShown = false;

	UPROPERTY(config)
	bool bRankedMatchWasChosenLastTime = false;
};
