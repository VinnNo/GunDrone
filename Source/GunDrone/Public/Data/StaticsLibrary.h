// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StaticsLibrary.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
namespace EEntityStates
{
	enum Type
	{
		Idle    UMETA(DisplayName = "Idle"),
		Patrol  UMETA(DisplayName = "Patrol"),
		Chase   UMETA(DisplayName = "Chase"),
		Engage  UMETA(DisplayName = "Engage"),
	};
}

UENUM(BlueprintType)
namespace EEntityEngagement
{
	enum Type
	{
		Approach  UMETA(DisplayName = "Approach"),
		Strafe    UMETA(DisplayName = "Strafe"),
		Flee     UMETA(DisplayName = "Flee"),
	};
}

UENUM(BlueprintType)
namespace EEntityTargetRotation
{
	enum Type
	{
		None     UMETA(DisplayName = "None"),
		Actor    UMETA(DisplayName = "Actor"),
		Forward  UMETA(DisplayName = "Forward"),
	};
}


UCLASS()
class GUNDRONE_API UStaticsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
