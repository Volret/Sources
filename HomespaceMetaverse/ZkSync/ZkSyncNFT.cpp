// Fill out your copyright notice in the Description page of Project Settings.


#include "ZkSyncNFT.h"
#include "SocketIOClient.h"
#include "Misc/Base64.h"
#include "uuid_v4.h"

// Sets default values
AZkSyncNFT::AZkSyncNFT()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Socket = CreateDefaultSubobject<USocketIOClientComponent>(TEXT("SocketIOClientComponent"));
	Socket->bShouldAutoConnect = false;
}

// Called when the game starts or when spawned
void AZkSyncNFT::BeginPlay()
{
	Super::BeginPlay();

	if (!FModuleManager::Get().IsModuleLoaded("SocketIOClient"))
	{
		FModuleManager::Get().LoadModule("SocketIOClient");
	}

	if (!FModuleManager::Get().IsModuleLoaded("SocketIOLib"))
	{
		FModuleManager::Get().LoadModule("SocketIOLib");
	}

	if (!FModuleManager::Get().IsModuleLoaded("Json"))
	{
		FModuleManager::Get().LoadModule("Json");
	}

	if (!FModuleManager::Get().IsModuleLoaded("SIOJson"))
	{
		FModuleManager::Get().LoadModule("SIOJson");
	}

	InitConnection(URL, HandShake);
}

void AZkSyncNFT::InitConnection(FString InURL, FString InHandShake)
{
	Socket = Cast<USocketIOClientComponent>(GetComponentByClass(USocketIOClientComponent::StaticClass()));
	if (!Socket)
	{
		UE_LOG(LogTemp, Warning, TEXT("No socket IO component found"));
		return;
	}
	else
	{
		InitZkSyncEvents();
		InitConnectionEvents();

		USIOJsonObject* Query = NewObject<USIOJsonObject>();

		UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
		UUIDv4::UUID uuid = uuidGenerator.getUUID();

		std::string s = uuid.str();

		FString Auth = s.c_str();

		Query->SetStringField("token", Auth);

		Socket->Connect(URL, HandShake, Auth, Query);

		UE_LOG(LogTemp, Log, TEXT("Found SIOClientComponent: %s"), *Socket->GetDesc());
	}
}

void AZkSyncNFT::Disconnect()
{
	if (IsValid(Socket))
		Socket->Disconnect();
}
void AZkSyncNFT::InitZkSyncEvents()
{
	if (Socket)
	{
		Socket->OnNativeEvent(RequestConnectionEventName, [this](const FString& Event, const TSharedPtr<FJsonValue>& Message) -> void
			{
				if (!Message)
				{
					UE_LOG(LogTemp, Log, TEXT("[request_connection] Message pointer is null"));
					nullptr;
				}

				FRequestConnectionZkSync connection;

				if (USIOJConvert::JsonObjectToUStruct(Message->AsObject(), FRequestConnectionZkSync::StaticStruct(), &connection))
					RequestConnection(connection);
				else
					UE_LOG(LogTemp, Log, TEXT("[request_connection] Wrong Json to UStruct Convecration [FWCRequestConnection]"));
			});

		Socket->OnNativeEvent(WalletConnectedEventName, [this](const FString& Event, const TSharedPtr<FJsonValue>& Message) -> void
			{
				if (!Message)
				{
					UE_LOG(LogTemp, Log, TEXT("[wc_connected] Message pointer is null"));
					nullptr;
				}

				FWCConnectedZkSync Connected;

				if (USIOJConvert::JsonObjectToUStruct(Message->AsObject(), FWCConnectedZkSync::StaticStruct(), &Connected))
					WCConnected(Connected);
				else
					UE_LOG(LogTemp, Log, TEXT("[wc_connected] Wrong Json to UStruct Convecration [FWCConnected]"));
			});

		Socket->OnNativeEvent(GamePoapInitializedEventName, [this](const FString& Event, const TSharedPtr<FJsonValue>& Message) -> void
			{
				if (!Message)
				{
					UE_LOG(LogTemp, Log, TEXT("[game_poap_initiated] Message pointer is null"));
					nullptr;
				}

				FGamePoapInitializedZkSync Connected;

				if (USIOJConvert::JsonObjectToUStruct(Message->AsObject(), FGamePoapInitializedZkSync::StaticStruct(), &Connected))
					GamePoapInitialized(Connected);
				else
					UE_LOG(LogTemp, Log, TEXT("[game_poap_initiated] Wrong Json to UStruct Convecration [FWCConnected]"));
			});

		Socket->OnNativeEvent(GamePoapFinalizedEventName, [this](const FString& Event, const TSharedPtr<FJsonValue>& Message) -> void
			{
				if (!Message)
				{
					UE_LOG(LogTemp, Log, TEXT("[game_poap_finalized] Message pointer is null"));
					nullptr;
				}

				FGamePoapFinalizedZkSync Disconnected;

				if (USIOJConvert::JsonObjectToUStruct(Message->AsObject(), FGamePoapFinalizedZkSync::StaticStruct(), &Disconnected))
					GamePoapFinalized(Disconnected);
				else
					UE_LOG(LogTemp, Log, TEXT("[game_poap_finalized] Wrong Json to UStruct Convecration [FWCConnected]"));
			});

		Socket->OnNativeEvent(AlreadyClaimedEventName, [this](const FString& Event, const TSharedPtr<FJsonValue>& Message) -> void
			{
				if (!Message)
				{
					UE_LOG(LogTemp, Log, TEXT("[game_poap_finalized] Message pointer is null"));
					nullptr;
				}

				FAlreadyClaimedZkSync Claimed;

				if (USIOJConvert::JsonObjectToUStruct(Message->AsObject(), FAlreadyClaimedZkSync::StaticStruct(), &Claimed))
					AlreadyClaimed(Claimed);
				else
					UE_LOG(LogTemp, Log, TEXT("[game_poap_finalized] Wrong Json to UStruct Convecration [FWCConnected]"));
			});
	}
}

void AZkSyncNFT::InitConnectionEvents()
{
	if (Socket)
	{
		Socket->OnConnected.AddDynamic(this, &AZkSyncNFT::OnConnected);
		Socket->OnConnectionProblems.AddDynamic(this, &AZkSyncNFT::OnConnectionProblems);
		Socket->OnDisconnected.AddDynamic(this, &AZkSyncNFT::OnDisconnected);
		Socket->OnFail.AddDynamic(this, &AZkSyncNFT::OnFail);
	}
}

void AZkSyncNFT::RequestGiveNFT()
{
	Socket->Emit(GiveMeNftRequestName);
}

void AZkSyncNFT::OnFail()
{
	UE_LOG(LogTemp, Warning, TEXT("[OnFail]"));
}
void AZkSyncNFT::OnDisconnected(TEnumAsByte<ESIOConnectionCloseReason> Reason)
{
	UE_LOG(LogTemp, Warning, TEXT("[OnDisconnected]"));
}

void AZkSyncNFT::OnConnected(FString SocketId, FString SessionId, bool bIsReconnection)
{
	UE_LOG(LogTemp, Warning, TEXT("[OnConnected]: %s; IsReconnection - %d"), *SessionId, bIsReconnection);
}

void AZkSyncNFT::OnConnectionProblems(int32 Attempts, int32 NextAttemptInMs, float TimeSinceConnected)
{
		
}

void AZkSyncNFT::GamePoapInitialized_Implementation(const FGamePoapInitializedZkSync& Connection)
{
	
}

void AZkSyncNFT::GamePoapFinalized_Implementation(const FGamePoapFinalizedZkSync& Connection)
{
	
}

void AZkSyncNFT::AlreadyClaimed_Implementation(const FAlreadyClaimedZkSync& Connection)
{
	
}

void AZkSyncNFT::RequestConnection_Implementation(const FRequestConnectionZkSync& Connection)
{
	
}

void AZkSyncNFT::WCConnected_Implementation(const FWCConnectedZkSync& Connection)
{
	
}