// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "HSGameMode.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Net/OnlineEngineInterface.h"

#include "HSConstants.h"
#include "HSHelpers.h"
#include "HSGameState.h"
#include "HSPlayerController.h"
#include "HSPlayerCharacter.h"

AHSGameMode::AHSGameMode()
{
}

void AHSGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AHSGameMode::InitGameState()
{
	Super::InitGameState();
	GS = GetGameState<AHSGameState>();
}

void AHSGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void AHSGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AHSGameMode::SetMatchState(FName NewState)
{
	Super::SetMatchState(NewState);
}

void AHSGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AHSGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

FString AHSGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}

void AHSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AHSGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}