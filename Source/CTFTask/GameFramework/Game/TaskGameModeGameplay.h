// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskGameMode.h"
#include "TaskGameModeGameplay.generated.h"

class ATaskFlagZone;
class ATaskFlag;

/**
 * 
 */
UCLASS()
class CTFTASK_API ATaskGameModeGameplay : public ATaskGameMode
{
	GENERATED_BODY()

protected:

	FTimerHandle SetTeamTimerHandle;	

public:

	/** This is the match time */
	UPROPERTY(EditAnywhere)
	float MatchTime	= 120.f;
	
	ATaskGameModeGameplay();

	virtual void Tick(float DeltaSeconds) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;	

	virtual void Logout(AController* Exiting) override;

	virtual void HandleMatchHasStarted() override;

	/** Handle the player Captured flag behaviour */
	void PlayerCapturedFlag(const ATaskFlagZone* Zone) const;

	/** Handle the character respawn */
	void RequestRespawn(ACharacter* EliminatedCharacter, AController* EliminatedController);

	/** Team Setter */
	void SetTeam() const;

	/** Time Settet */
	void SetMatchTime(const float Time) const;

	/** Function to handle the end of the game */
	void EndGame() const;
	
};
