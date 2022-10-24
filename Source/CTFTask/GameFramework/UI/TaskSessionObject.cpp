// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskSessionObject.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "TaskMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "CTFTask/Engine/TaskGameInstance.h"

/* ----------------------------------------------------------------------------------------------------------------------------------*/
void UTaskSessionObject::Setup(FOnlineSessionSearchResult Session)
{
	CurrentSession = Session;
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;	

	ServerName->SetText(FText::FromString(Session.Session.OwningUserName));

	const int32 CurrentPlayers = CurrentSession.Session.SessionSettings.NumPublicConnections - CurrentSession.Session.NumOpenPublicConnections;

	const int32 MaxPlayers = CurrentSession.Session.SessionSettings.NumPublicConnections;
	
	SessionPlayers->SetText(FText::FromString(FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers)));

	if(CurrentPlayers == MaxPlayers)
	{
		JoinButton->SetIsEnabled(false);
	}
	
	const UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
}

/* ----------------------------------------------------------------------------------------------------------------------------------*/
bool UTaskSessionObject::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	if(JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	const UTaskGameInstance* GameInstance = Cast<UTaskGameInstance>(GetGameInstance());

	if(GameInstance)
	{
		const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		if (Subsystem)
		{
			SessionInterface = Subsystem->GetSessionInterface();
			if (SessionInterface)
			{
				SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UTaskSessionObject::OnSessionJoined);	
			}
		}		
	}
	
	return true;;
}

/* ----------------------------------------------------------------------------------------------------------------------------------*/
void UTaskSessionObject::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);
	Joining->SetVisibility(ESlateVisibility::Visible);
	UTaskGameInstance* GameInstance = Cast<UTaskGameInstance>(GetGameInstance());
	
	if(GameInstance)
	{
		GameInstance->Join(CurrentSession);		
	}
}

/* ----------------------------------------------------------------------------------------------------------------------------------*/
void UTaskSessionObject::OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	if(JoinResult != EOnJoinSessionCompleteResult::Success)
	{
		JoinButton->SetIsEnabled(false);
		Joining->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	UTaskGameInstance* GameInstance = Cast<UTaskGameInstance>(GetGameInstance());
	
	FString Address;	

	if (!SessionInterface->GetResolvedConnectString(SessionName, Address) || !GameInstance)
	{
		return;
	}	

	APlayerController* PlayerController = GameInstance->GetFirstLocalPlayerController();
	if (!PlayerController)
	{
		return;
	}
	
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
