// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

/* --------------------------------------------------------------------------------------------------------------------------------------*/
void UTaskGameInstance::Init()
{
	Super::Init();
	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface)
		{
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UTaskGameInstance::OnSessionDestroyed);
		}
	}
}

/* --------------------------------------------------------------------------------------------------------------------------------------*/
void UTaskGameInstance::SetSession(int32 NumPublicConnections) const
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = NumPublicConnections;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;		
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;	
	SessionSettings.Set(TEXT("ServerName"), FString(TEXT("GameCanTask")),EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->CreateSession(0, SessionDefaultName, SessionSettings);
}

/* -----------------------------------------------------------------------------------------------------------*/
void UTaskGameInstance::DestroySession(FName SessionName) const
{
	SessionInterface->DestroySession(SessionDefaultName);
}

/* -----------------------------------------------------------------------------------------------------------*/
void UTaskGameInstance::Host(int32 NumPublicConnections)
{
	PublicConnections = NumPublicConnections;
	const auto CreateSession = SessionInterface->GetNamedSession(SessionDefaultName);

	if (CreateSession)
	{
		DestroySession(SessionDefaultName);
	}
	else
	{
		SetSession(NumPublicConnections);
	}	
}

/* -----------------------------------------------------------------------------------------------------------*/
void UTaskGameInstance::Join(const FOnlineSessionSearchResult& SessionToJoin) const
{ 
	SessionInterface->JoinSession(0, SessionDefaultName, SessionToJoin);
}

/* -----------------------------------------------------------------------------------------------------------*/
void UTaskGameInstance::Search()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->bIsLanQuery =  false;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE,true,EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

/* -----------------------------------------------------------------------------------------------------------*/
void UTaskGameInstance::OnSessionDestroyed(FName SessionName, bool Success)
{
	if (!Success)
	{
		return;
	}	
}