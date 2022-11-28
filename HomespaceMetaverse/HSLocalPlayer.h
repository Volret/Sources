#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "HSLocalPlayer.generated.h"

UCLASS()
class HOMESPACEMETAVERSE_API UHSLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
	
public:
	virtual FString GetNickname() const override;
};

