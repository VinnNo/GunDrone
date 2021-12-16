// Copyright Epic Games, Inc. All Rights Reserved.

#include "GunDroneGameMode.h"
#include "GunDroneHUD.h"
#include "GunDroneCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGunDroneGameMode::AGunDroneGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGunDroneHUD::StaticClass();
}
