// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFTASK/GameFramework/UI/TaskCharacterOverlay.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

/* -----------------------------------------------------------------------------------------------------------*/
void UTaskCharacterOverlay::SetHealth(const float Health) const
{
	if(HealthBar)
	{
		HealthBar->SetPercent(Health);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void UTaskCharacterOverlay::SetFlagVisibility(const bool bIsVisible) const
{
	if(FlagImage)
	{
		FlagImage->SetVisibility( bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void UTaskCharacterOverlay::SetRedTeamScore(int32 TeamRedScore) const
{
	if(RedScore)
	{
		const FString RedScoreText = FString::Printf(TEXT("%d"), TeamRedScore);		
		RedScore->SetText(FText::FromString(RedScoreText));
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void UTaskCharacterOverlay::SetBlueTeamScore(int32 TeamBlueScore) const
{
	if(BlueScore)
	{
		const FString BlueScoreText = FString::Printf(TEXT("%d"), TeamBlueScore);		
		BlueScore->SetText(FText::FromString(BlueScoreText));
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void UTaskCharacterOverlay::SetTimer(float Time) const
{
	if(MatchTime && Time > 0.f)
	{
		const int32 Minutes = FMath::FloorToInt(Time / 60.f);
		const int32 Seconds = Time - Minutes * 60;

		const FString TimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		MatchTime->SetText(FText::FromString(TimeText));
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void UTaskCharacterOverlay::SetAnnouncement(const FString AnnouncementText) const
{
	if(Announcement)
	{
		Announcement->SetVisibility(ESlateVisibility::Visible);
		Announcement->SetText(FText::FromString(AnnouncementText));
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void UTaskCharacterOverlay::SetDead(const bool bIsDead) const
{
	if(DeadAnnouncementBorder)
	{
		DeadAnnouncementBorder->SetVisibility(bIsDead ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
