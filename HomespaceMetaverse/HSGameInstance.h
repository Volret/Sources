#pragma once

#include "Engine/GameInstance.h"
#include "Misc/ConfigCacheIni.h"
#include "HSGameInstance.generated.h"

UCLASS()
class HOMESPACEMETAVERSE_API UHSGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	UHSGameInstance();

	virtual void OnStart() override;

public:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void StartGameInstance() override;

	UFUNCTION(BlueprintCallable)
	class UHSLocalPlayer* GetLocalPlayer() const;

	void InitSettings();


	virtual TSubclassOf<UOnlineSession> GetOnlineSessionClass() override;
};
