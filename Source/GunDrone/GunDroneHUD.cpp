// Copyright Epic Games, Inc. All Rights Reserved.

#include "GunDroneHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AGunDroneHUD::AGunDroneHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture> CrosshairTexObj(TEXT("/Game/GunDrone/Art/Textures/Crosshairs/crosshair177.crosshair177"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshObject(TEXT("/Game/Geometry/SM_DroneBody.SM_DroneBody"));
	CrosshairTex = CrosshairTexObj.Object;
}


void AGunDroneHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X - 32.0f),
										   (Center.Y - 32.0f));

	// draw the crosshair
	if (IsValid(CrosshairTex))
	{

		FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}
