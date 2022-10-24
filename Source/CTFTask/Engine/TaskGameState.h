// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTFTask/GameFramework/Game/TaskPlayerState.h"
#include "GameFramework/GameState.h"
#include "TaskGameState.generated.h"

/**
 *  The GameState handle the current state of the game
 */
UCLASS()
class CTFTASK_API ATaskGameState : public AGameState
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	

	// Properties	
	
	/** Stores the team players */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<ATaskPlayerState*> RedTeam;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<ATaskPlayerState*> BlueTeam;

	/** Stores the Red Team Score  */
	UPROPERTY(ReplicatedUsing = OnRep_RedTeamScore)
	float RedTeamScore;
	
	/** Stores the Blue Team Score */
	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamScore)
	float BlueTeamScore;

	/** Stores the Blue Team Score */
	UPROPERTY(ReplicatedUsing = OnRep_MatchTime)
	float MatchTime;

	/** Match announcement to be displayed */
	UPROPERTY(ReplicatedUsing = OnRep_MatchAnnouncement)
	FString MatchAnnouncement;

	// Functions

	/** Function to handle the Red Team Score */
	void RedTeamScores();

	/** Function to handle the Blue Team Score */
	void BlueTeamScores();

	// Function to handle the match time
	void SetMatchTime(const float Time);

	float GetMatchTime() { return MatchTime; }

	// Function to handle the End of the game
	void EndGame();
	
	UFUNCTION()
	void OnRep_RedTeamScore() const;

	UFUNCTION()
	void OnRep_BlueTeamScore() const;

	UFUNCTION()
	void OnRep_MatchTime() const;

	UFUNCTION()
	void OnRep_MatchAnnouncement() const;
	
};
