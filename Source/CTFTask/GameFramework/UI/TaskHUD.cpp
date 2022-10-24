// Copyright Epic Games, Inc. All Rights Reserved.

#include "TaskHUD.h"
#include "Engine/AssetManager.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "CanvasItem.h"
#include "TaskCharacterOverlay.h"

/* -----------------------------------------------------------------------------------------------------------*/
ATaskHUD::ATaskHUD()
{

}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskHUD::BeginPlay()
{
	if (!CrosshairTexture.ToString().IsEmpty())
	{
		CrosshairTex = CrosshairTexture.LoadSynchronous();
	}

	AddCharacterOverlay();
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskHUD::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTex)
	{
		// Draw a simple crosshair

		// find center of the Canvas
		const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
		const FVector2D CrosshairDrawPosition((Center.X), (Center.Y + 20.0f));

		// draw the crosshair
		FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskHUD::AddCharacterOverlay()
{

	APlayerController* PlayerController = GetOwningPlayerController();
	if(PlayerController && CharacterOverlayWidget)
	{
		CharacterOverlay = CreateWidget<UTaskCharacterOverlay>(PlayerController, CharacterOverlayWidget);
		CharacterOverlay->AddToViewport();
	}		
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskHUD::SetHealth(const float Percent)
{
	if(CharacterOverlay)
	{
		CharacterOverlay->SetHealth(Percent);		
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskHUD::SetFlagVisibility(bool bIsVisible)
{
	if(CharacterOverlay)
	{
		CharacterOverlay->SetFlagVisibility(bIsVisible);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskHUD::SetRedTeamScore(int32 RedScore)
{
	if(CharacterOverlay)
	{
		CharacterOverlay->SetRedTeamScore(RedScore);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskHUD::SetBlueTeamScore(int32 BlueScore)
{
	if(CharacterOverlay)
	{
		CharacterOverlay->SetBlueTeamScore(BlueScore);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskHUD::SetTimer(float Time)
{
	if(CharacterOverlay)
	{
		CharacterOverlay->SetTimer(Time);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskHUD::SetAnnouncement(const FString AnnouncementText)
{
	if(CharacterOverlay)
	{
		CharacterOverlay->SetAnnouncement(AnnouncementText);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskHUD::SetDead(const bool bIsDead)
{
	if(CharacterOverlay)
	{
		CharacterOverlay->SetDead(bIsDead);
	}
}
