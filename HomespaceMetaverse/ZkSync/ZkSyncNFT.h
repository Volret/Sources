// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SocketIOClientComponent.h"
#include "GameFramework/Actor.h"
#include "SocketIONative.h"
#include "ZkSyncNFT.generated.h"

USTRUCT(Blueprintable)
struct FGamePoapInitializedZkSync
{
	GENERATED_BODY()
};

USTRUCT(Blueprintable)
struct FGamePoapFinalizedZkSync
{
	GENERATED_BODY()
};

USTRUCT(Blueprintable)
struct FAlreadyClaimedZkSync
{
	GENERATED_BODY()
};

USTRUCT(Blueprintable)
struct FWCConnectedZkSync
{
	GENERATED_BODY()
};

USTRUCT(Blueprintable)
struct FRequestConnectionZkSync
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
		FString Url;
};

UCLASS()
class HOMESPACEMETAVERSE_API AZkSyncNFT : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZkSyncNFT();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void Disconnect();

	UFUNCTION(BlueprintCallable)
	void InitConnection(FString InURL, FString InHandShake);
	void InitZkSyncEvents();
	void InitConnectionEvents();

	//
	UFUNCTION()
	void OnFail();
	UFUNCTION()
	void OnDisconnected(TEnumAsByte<ESIOConnectionCloseReason> Reason);
	UFUNCTION()
	void OnConnectionProblems(int32 Attempts, int32 NextAttemptInMs, float TimeSinceConnected);
	UFUNCTION()
	void OnConnected(FString SocketId, FString SessionId, bool bIsReconnection);

	UFUNCTION(BlueprintNativeEvent)
		void RequestConnection(const FRequestConnectionZkSync& Connection);
	virtual void RequestConnection_Implementation(const FRequestConnectionZkSync& Connection);

	UFUNCTION(BlueprintNativeEvent)
		void WCConnected(const FWCConnectedZkSync& Connection);
	virtual void WCConnected_Implementation(const FWCConnectedZkSync& Connection);

	UFUNCTION(BlueprintNativeEvent)
		void GamePoapInitialized(const FGamePoapInitializedZkSync& Connection);
	virtual void GamePoapInitialized_Implementation(const FGamePoapInitializedZkSync& Connection);

	UFUNCTION(BlueprintNativeEvent)
		void GamePoapFinalized(const FGamePoapFinalizedZkSync& Connection);
	virtual void GamePoapFinalized_Implementation(const FGamePoapFinalizedZkSync& Connection);

	UFUNCTION(BlueprintNativeEvent)
		void AlreadyClaimed(const FAlreadyClaimedZkSync& Connection);
	virtual void AlreadyClaimed_Implementation(const FAlreadyClaimedZkSync& Connection);

	UFUNCTION(BlueprintCallable)
		void RequestGiveNFT();


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient)
		FString URL = "wss://poker.homespace.is";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient)
		FString HandShake = "poker";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient)
		FString RequestConnectionEventName = "request_connection";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient)
		FString WalletConnectedEventName = "wc_connected";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient)
		FString AlreadyClaimedEventName = "game_poap_already_claimed";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient)
		FString GiveMeNftRequestName = "request_give_nft";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient)
		FString GamePoapInitializedEventName = "game_poap_initiated";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient)
		FString GamePoapFinalizedEventName = "game_poap_finalized";
	
	UPROPERTY()
		USocketIOClientComponent* Socket; //get a reference or add as subobject in your actor
};
