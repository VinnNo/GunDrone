// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GunDroneHUD.generated.h"

UCLASS()
class AGunDroneHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGunDroneHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture* CrosshairTex;

};

