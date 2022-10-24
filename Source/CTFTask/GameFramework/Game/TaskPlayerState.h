// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Team.h"
#include "GameFramework/PlayerState.h"
#include "TaskPlayerState.generated.h"

class ACTFTaskCharacter;
class ATaskPlayerController;

/**
 * This is the player state for the player
 */
UCLASS()
class CTFTASK_API ATaskPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	

private:
	/** Player State Character Reference */
	UPROPERTY()
	ACTFTaskCharacter* CharacterRef;

	/** PlayerState Player Controller Reference	 */
	UPROPERTY(Replicated)
	ATaskPlayerController* PlayerControllerRef;

	UPROPERTY(ReplicatedUsing = OnRep_Team, Transient)
	ETeam Team = ETeam::ET_NoTeam;

	UFUNCTION()
	void OnRep_Team();	

public:
	/** Team Getter	 */
	UFUNCTION(BlueprintCallable)
	ETeam GetTeam() const { return Team; }

	/** Team Setter */
	UFUNCTION(BlueprintCallable)
	void SetTeam(const ETeam NewTeam);
};
