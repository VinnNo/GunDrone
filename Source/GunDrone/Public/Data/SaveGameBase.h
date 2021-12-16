// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameBase.generated.h"

/**
 * 
 */
UCLASS()
class GUNDRONE_API USaveGameBase : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString PlayerSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Player)
		float PlayerHealthMax;

	UPROPERTY(VisibleAnywhere, Category = Player)
		float PlayerHealthBoosted;

	UPROPERTY(VisibleAnywhere, Category = Player)
		int PlayerJumpCountMax;

	UPROPERTY(VisibleAnywhere, Category = Player)
		int PlayerJumpCountBoosted;

	USaveGameBase();
};
