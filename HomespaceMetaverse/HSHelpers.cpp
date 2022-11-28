// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSHelpers.h"

#include "GameFramework/GameSession.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"

#include "HSPlayerController.h"
#include "HSGameUserSettings.h"
#include "HSPlayerCharacter.h"
#include "HSGameState.h"
#include "HSLocalPlayer.h"
#include "HSGameInstance.h"

bool HSHelpers::IsDedicatedServer(const UObject* WorldContextObject)
{
	return IsRunningDedicatedServer() || (IsValid(WorldContextObject) && IsValid(WorldContextObject->GetWorld()) && WorldContextObject->GetWorld()->IsNetMode(NM_DedicatedServer));
}