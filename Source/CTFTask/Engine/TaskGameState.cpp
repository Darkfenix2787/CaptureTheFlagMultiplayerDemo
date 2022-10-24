// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameState.h"

#include "CTFTask/GameFramework/Character/TaskPlayerController.h"
#include "Net/UnrealNetwork.h"

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATaskGameState, RedTeamScore);
	DOREPLIFETIME(ATaskGameState, BlueTeamScore);
	DOREPLIFETIME(ATaskGameState, MatchTime);
	DOREPLIFETIME(ATaskGameState, MatchAnnouncement);
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameState::RedTeamScores()
{
	++RedTeamScore;
	ATaskPlayerController* PlayerControllerRef = Cast<ATaskPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerControllerRef)
	{
		PlayerControllerRef->SetHUDRedTeamScore(RedTeamScore);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameState::BlueTeamScores()
{
	++BlueTeamScore;
	ATaskPlayerController* PlayerControllerRef = Cast<ATaskPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerControllerRef)
	{
		PlayerControllerRef->SetHUDBlueTeamScore(BlueTeamScore);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameState::OnRep_RedTeamScore() const
{
	ATaskPlayerController* PlayerControllerRef = Cast<ATaskPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerControllerRef)
	{
		PlayerControllerRef->SetHUDRedTeamScore(RedTeamScore);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameState::OnRep_BlueTeamScore() const
{
	ATaskPlayerController* PlayerControllerRef = Cast<ATaskPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerControllerRef)
	{
		PlayerControllerRef->SetHUDBlueTeamScore(BlueTeamScore);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameState::SetMatchTime(const float Time)
{
	MatchTime = Time;
	ATaskPlayerController* PlayerControllerRef = Cast<ATaskPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerControllerRef)
	{
		PlayerControllerRef->SetHUDTimer(MatchTime);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameState::OnRep_MatchTime() const
{
	ATaskPlayerController* PlayerControllerRef = Cast<ATaskPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerControllerRef)
	{
		PlayerControllerRef->SetHUDTimer(MatchTime);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameState::EndGame()
{
	FString AnnouncementText = FString("");
	
	if(BlueTeamScore > RedTeamScore)
	{
		AnnouncementText = FString("Blue Team Wins");		
	}

	if(RedTeamScore > BlueTeamScore)
	{
		AnnouncementText = FString("Red Team Wins");
	}

	if(RedTeamScore == BlueTeamScore)
	{
		AnnouncementText = FString("Tie Match");
	}

	MatchAnnouncement = AnnouncementText;
	ATaskPlayerController* PlayerControllerRef = Cast<ATaskPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerControllerRef)
	{
		PlayerControllerRef->SetHUDAnnouncement(MatchAnnouncement);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameState::OnRep_MatchAnnouncement() const
{
	ATaskPlayerController* PlayerControllerRef = Cast<ATaskPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerControllerRef)
	{
		PlayerControllerRef->SetHUDAnnouncement(MatchAnnouncement);
	}
}
