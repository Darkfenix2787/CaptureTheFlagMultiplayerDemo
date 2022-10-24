// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TaskHUD.generated.h"

class UTexture2D;
class UTaskCharacterOverlay;

UCLASS()
class ATaskHUD : public AHUD
{
	GENERATED_BODY()
	

protected:

	// Properties

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> CrosshairTexture;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTaskCharacterOverlay> CharacterOverlayWidget;

	UPROPERTY()
	UTexture2D* CrosshairTex = nullptr;
	
	UTaskCharacterOverlay* CharacterOverlay;

	// Functions
	
	ATaskHUD();	

	virtual void BeginPlay() override;

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	/** Adds the HUD to player */
	void AddCharacterOverlay();

public:

	/** Hud Setters */

	void SetHealth(const float Percent);

	void SetFlagVisibility(bool bIsVisible);

	void SetRedTeamScore(int32 RedScore);

	void SetBlueTeamScore(int32 BlueScore);

	void SetTimer(float Time);

	void SetAnnouncement(const FString AnnouncementText);

	void SetDead(const bool bIsDead);
};

