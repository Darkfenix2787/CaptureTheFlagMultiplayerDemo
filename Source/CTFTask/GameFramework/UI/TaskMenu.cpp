// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskMenu.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "TaskServerList.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "CTFTask/Engine/TaskGameInstance.h"
#include "CTFTask/GameFramework/Game/TaskGameModeMainMenu.h"
#include "Kismet/KismetSystemLibrary.h"

class ATaskGameModeMainMenu;
class IOnlineSubsystem;
/* -------------------------------------------------------------------------------------------------------------------------------------------*/
void UTaskMenu::MenuSetup(int32 NumberOfPublicConnections, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

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

/* -------------------------------------------------------------------------------------------------------------------------------------------*/
bool UTaskMenu::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	if(CreateGameButton)
	{
		CreateGameButton->OnClicked.AddDynamic(this, &ThisClass::CreateGameButtonClicked);
	}

	if(BrowseGamesButton)
	{
		BrowseGamesButton->OnClicked.AddDynamic(this, &ThisClass::BrowseGamesButtonClicked);
	}
	
	if(QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &ThisClass::QuitGameButtonClicked);
	}

	GameInstance = Cast<UTaskGameInstance>(GetGameInstance());

	if(GameInstance)
	{
		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		if (Subsystem)
		{
			SessionInterface = Subsystem->GetSessionInterface();
			if (SessionInterface)
			{
				SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UTaskMenu::OnSessionCreated);
				SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UTaskMenu::OnSessionFound);			
			}
		}		
	}
	return true;
}

/* -------------------------------------------------------------------------------------------------------------------------------------------*/
void UTaskMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	MenuTearDown();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

/* -------------------------------------------------------------------------------------------------------------------------------------------*/
void UTaskMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController)
		{
			FInputModeGameOnly InputModeGameOnlyData;
			PlayerController->SetInputMode((InputModeGameOnlyData));
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

/* -------------------------------------------------------------------------------------------------------------------------------------------*/
void UTaskMenu::CreateGameButtonClicked()
{
	CreateGameButton->SetIsEnabled(false);
	ContextMenuSwitcher->SetActiveWidgetIndex(0.f);	
	if(GameInstance)
	{
		GameInstance->Host(NumPublicConnections);
	}
}

/* -------------------------------------------------------------------------------------------------------------------------------------------*/
void UTaskMenu::BrowseGamesButtonClicked()
{
	BrowseGamesButton->SetIsEnabled(false);
	ContextMenuSwitcher->SetActiveWidgetIndex(1.f);
	if(GameInstance)
	{
		GameInstance->Search();
	}
}

/* -------------------------------------------------------------------------------------------------------------------------------------------*/
void UTaskMenu::QuitGameButtonClicked()
{
	const UWorld* World = GetWorld();

	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController)
		{
			UKismetSystemLibrary::QuitGame(World, PlayerController,EQuitPreference::Quit,true);		
		}		
	}
}

/* -------------------------------------------------------------------------------------------------------------------------------------------*/
void UTaskMenu::OnSessionCreated(FName SessionName, bool Success) const
{
	if(Success)
	{
		ATaskGameModeMainMenu* GameModeRef = GetWorld()->GetAuthGameMode<ATaskGameModeMainMenu>();
		if(GameModeRef)
		{
			GameModeRef->TravelToLobby(PathToLobby);
		}			
	}
	else
	{
		CreateGameButton->SetIsEnabled(true);
	}
}

/* -------------------------------------------------------------------------------------------------------------------------------------------*/
void UTaskMenu::OnSessionFound(bool Success) const
{
	if(GameInstance)
	{
		const TSharedPtr<FOnlineSessionSearch> SessionSearch = GameInstance->SessionSearch;

		if(SessionSearch)
		{
			ServerList->PopulateListServer(SessionSearch->SearchResults);
		}	
	}

	BrowseGamesButton->SetIsEnabled(true);
}
