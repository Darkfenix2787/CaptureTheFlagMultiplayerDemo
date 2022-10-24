// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskPlayerState.h"

#include "CTFTask/GameFramework/Character/TaskCharacter.h"
#include "CTFTask/GameFramework/Character/TaskPlayerController.h"
#include "Net/UnrealNetwork.h"

/* --------------------------------------------------------------------------------------------------------------------------------------*/
void ATaskPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATaskPlayerState,Team);
	DOREPLIFETIME(ATaskPlayerState,PlayerControllerRef);	
}

/* --------------------------------------------------------------------------------------------------------------------------------------*/
void ATaskPlayerState::SetTeam(const ETeam NewTeam)
{
	Team = NewTeam;

	CharacterRef = CharacterRef == nullptr ? Cast<ACTFTaskCharacter>(GetPawn()) : CharacterRef;
	if(CharacterRef)
	{
		CharacterRef->SetTeamColor(Team);
		PlayerControllerRef = PlayerControllerRef == nullptr ? Cast<ATaskPlayerController>(CharacterRef->Controller) : PlayerControllerRef;
	}	
}

/* --------------------------------------------------------------------------------------------------------------------------------------*/
void ATaskPlayerState::OnRep_Team()
{
	CharacterRef = CharacterRef == nullptr ? Cast<ACTFTaskCharacter>(GetPawn()) : CharacterRef;

	if(CharacterRef)
	{
		CharacterRef->SetTeamColor(Team);
		PlayerControllerRef = PlayerControllerRef == nullptr ? Cast<ATaskPlayerController>(CharacterRef->Controller) : PlayerControllerRef;		
	}	
}