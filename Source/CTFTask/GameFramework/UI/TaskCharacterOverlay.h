// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TaskCharacterOverlay.generated.h"

class UImage;
class UBorder;
class UTextBlock;
class UProgressBar;
/**
 * User Main Widget
 */
UCLASS()
class CTFTASK_API UTaskCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
public:	

	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* RedScore;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* BlueScore;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* MatchTime;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* Announcement;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UBorder* DeadAnnouncementBorder;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* FlagImage;

	/** Hud Setters */

	void SetHealth(const float Health) const;

	void SetFlagVisibility (const bool bIsVisible) const;

	void SetRedTeamScore(int32 TeamRedScore) const;

	void SetBlueTeamScore(int32 TeamBlueScore) const;

	void SetTimer(float Time) const;

	void SetAnnouncement(const FString TimeText) const;

	void SetDead(const bool bIsDead) const;
	
};
