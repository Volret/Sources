// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HSConstants.h"
#include "GameFramework/PlayerState.h"
#include "HSPlayerState.generated.h"

UCLASS()
class HOMESPACEMETAVERSE_API AHSPlayerState : public APlayerState
{
	GENERATED_BODY()

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	AHSPlayerState();
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(Replicated)
	uint64 BackendPlayerId = 0;

	class AHSPlayerCharacter* GetPlayerCharacter();

	class AHSPlayerController* GetController() const;

protected:
	/** Copy properties which need to be saved in inactive PlayerState */
	virtual void CopyProperties(APlayerState* PlayerState) override;
};
