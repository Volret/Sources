#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "HSPlayerController.h"
#include "GameFramework/GameMode.h"
#include "OnlineSubsystemTypes.h"
#include "HSGameMode.generated.h"

TEMP_UNIQUENETIDSTRING_SUBCLASS(FUniqueNetIdNullCustom, NULL_SUBSYSTEM);

UCLASS(minimalapi)
class AHSGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AHSGameMode();
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override; 
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void InitGameState() override;
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void SetMatchState(FName NewState);
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void Tick(float DeltaSeconds) override;
	

	UPROPERTY()
	class AHSGameState* GS;

	UPROPERTY()
	TArray<AHSPlayerController*> Players;

protected:
	virtual void BeginPlay() override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;
};



