// Fill out your copyright notice in the Description page of Project Settings.


#include "HSPlayerState.h"

#include "HSGameMode.h"
#include "Stats/Stats.h"
#include "HSPlayerController.h"
#include "HSPlayerCharacter.h"
#include "Net/UnrealNetwork.h"

AHSPlayerState::AHSPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHSPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHSPlayerState, BackendPlayerId);
}

AHSPlayerCharacter* AHSPlayerState::GetPlayerCharacter()
{
	return GetPawn<AHSPlayerCharacter>();
}

AHSPlayerController* AHSPlayerState::GetController() const
{
	return Cast<AHSPlayerController>(GetOwner());
}

void AHSPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	if (AHSPlayerState* TargetPlayerState = Cast<AHSPlayerState>(PlayerState))
	{
		TargetPlayerState->BackendPlayerId = BackendPlayerId;
	}
}

void AHSPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}