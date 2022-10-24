// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameModeGameplay.h"

#include "TaskFlagZone.h"
#include "CTFTask/Engine/TaskGameState.h"
#include "CTFTask/GameFramework/Character/TaskCharacter.h"
#include "CTFTask/GameFramework/UI/TaskHUD.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

/* -----------------------------------------------------------------------------------------------------------*/
ATaskGameModeGameplay::ATaskGameModeGameplay() 
{
	HUDClass = ATaskHUD::StaticClass();
	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameModeGameplay::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(SetTeamTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(SetTeamTimerHandle,this,&ATaskGameModeGameplay::SetTeam,0.4f,false);
	}

	SetTeam();	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameModeGameplay::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ATaskGameState* GameStateRef = Cast<ATaskGameState>(UGameplayStatics::GetGameState(this));
	ATaskPlayerState* PlayerStateRef = Cast<ATaskPlayerState>(Exiting->GetPlayerState<ATaskPlayerState>());

	if (GameStateRef && PlayerStateRef)
	{
		if (GameStateRef->RedTeam.Contains(PlayerStateRef))
		{
			GameStateRef->RedTeam.Remove(PlayerStateRef);
		}
		
		if (GameStateRef->BlueTeam.Contains(PlayerStateRef))
		{
			GameStateRef->BlueTeam.Remove(PlayerStateRef);
		}
	}
	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameModeGameplay::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MatchTime -= DeltaSeconds;
	SetMatchTime(MatchTime);

	if(MatchTime <= 0.f)
	{
		EndGame();
	}	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameModeGameplay::PlayerCapturedFlag(const ATaskFlagZone* Zone) const
{
	ATaskGameState* GameStateRef = Cast<ATaskGameState>(GameState);
	if (GameStateRef)
	{
		if (Zone->Team == ETeam::ET_BlueTeam)
		{
			GameStateRef->BlueTeamScores();
		}
		if (Zone->Team == ETeam::ET_RedTeam)
		{
			GameStateRef->RedTeamScores();
		}
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameModeGameplay::RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController)
{
	if(EliminatedCharacter)
	{
		EliminatedCharacter->Reset();
		EliminatedCharacter->Destroy();		
	}

	if(EliminatedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		const int32 Selection = FMath::RandRange(0,PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(EliminatedController,PlayerStarts[Selection]);		
	}		
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameModeGameplay::HandleMatchHasStarted()
{	
	Super::HandleMatchHasStarted();	

	SetTeam();	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameModeGameplay::SetTeam() const
{
	ATaskGameState* GameStateRef = Cast<ATaskGameState>(UGameplayStatics::GetGameState(this));

	if(GameStateRef)
	{
		for (const auto PlayerState : GameStateRef->PlayerArray)
		{
			ATaskPlayerState* PlayerStateRef = Cast<ATaskPlayerState>(PlayerState);
			if(PlayerStateRef && PlayerStateRef->GetTeam() == ETeam::ET_NoTeam)
			{
				if (GameStateRef->BlueTeam.Num() >= GameStateRef->RedTeam.Num())
				{
					GameStateRef->RedTeam.AddUnique(PlayerStateRef);
					PlayerStateRef->SetTeam(ETeam::ET_RedTeam);					
				}
				else
				{				
					GameStateRef->BlueTeam.AddUnique(PlayerStateRef);
					PlayerStateRef->SetTeam(ETeam::ET_BlueTeam);					
				}
			}	
		}		
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameModeGameplay::SetMatchTime(const float Time) const
{
	ATaskGameState* GameStateRef = Cast<ATaskGameState>(GameState);
	if (GameStateRef)
	{
		GameStateRef->SetMatchTime(Time);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskGameModeGameplay::EndGame() const
{
	ATaskGameState* GameStateRef = Cast<ATaskGameState>(GameState);
	if (GameStateRef)
	{
		GameStateRef->EndGame();
	}
}
