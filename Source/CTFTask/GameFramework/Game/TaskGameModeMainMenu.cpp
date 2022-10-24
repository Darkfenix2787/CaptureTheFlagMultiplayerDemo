// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameModeMainMenu.h"

void ATaskGameModeMainMenu::TravelToLobby(const FString PathToLobby)
{
	UWorld* GameWorld = GetWorld();
	if(GameWorld)
	{
		GameWorld->ServerTravel(PathToLobby);	
	}	
}
