// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"

#include "HSPlayerState.h"
#include "Animation/BlendSpace.h"

class HSHelpers
{
public:
	static bool IsDedicatedServer(const UObject* WorldContextObject);
};