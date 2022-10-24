// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameModeLobby.h"
#include "CTFTask/Engine/TaskGameInstance.h"
#include "GameFramework/GameStateBase.h"

/* --------------------------------------------------------------------------------------------------------------------------------------*/
void ATaskGameModeLobby::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	GameInstanceRef = Cast<UTaskGameInstance>(GetGameInstance());
	UWorld* World = GetWorld();
	if(!World || !GameInstanceRef)
	{
		return;
	}
	
	const int32 NumberOfPlayers = GameState->PlayerArray.Num();
	PathToGame = FString::Printf(TEXT("%s?listen"), *PathToGame);
	PathToMainMenu = FString::Printf(TEXT("%s?listen"), *PathToMainMenu);
	
	GetWorld()->GetTimerManager().ClearTimer(WaitPlayersHandle);
	GetWorld()->GetTimerManager().SetTimer(WaitPlayersHandle,this, &ATaskGameModeLobby::LogOutPlayer,180.0f,false);
	
	if(NumberOfPlayers == GameInstanceRef->PublicConnections)
	{
		GetWorld()->GetTimerManager().ClearTimer(WaitPlayersHandle);
		bUseSeamlessTravel = true;
		World->ServerTravel(PathToGame);		
	}
}

/* --------------------------------------------------------------------------------------------------------------------------------------*/
void ATaskGameModeLobby::LogOutPlayer()
{
	if(GameInstanceRef)
	{
		GameInstanceRef->DestroySession(SessionDefaultName);		
	}
	
	UWorld* World = GetWorld();
	if(World)
	{
		bUseSeamlessTravel = true;
		World->ServerTravel(PathToMainMenu);
	}
}