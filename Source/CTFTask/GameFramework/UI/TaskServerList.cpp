// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskServerList.h"

#include "OnlineSessionSettings.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "TaskSessionObject.h"
#include "CTFTask/Engine/TaskGameInstance.h"
#include "Internationalization/TextFormatter.h"

/* -------------------------------------------------------------------------------------------------------------------------------------------*/
void UTaskServerList::PopulateListServer(TArray<FOnlineSessionSearchResult> SearchResults)
{
	ServerList->ClearChildren();
	const UTaskGameInstance* GameInstance = Cast<UTaskGameInstance>(GetGameInstance());
	if(GameInstance && SearchResults.Num() > 0)
	{
		const FText ServersText = FText::FromString(FString(FString::FromInt(SearchResults.Num()) + " Servers Found"));
		Servers->SetText(ServersText);
		
		for (const FOnlineSessionSearchResult CurrentResult : SearchResults)
		{
			if(SessionObjectWidget)
			{
				UTaskSessionObject* SessionObjectWidgetRef = CreateWidget<UTaskSessionObject>(this,SessionObjectWidget);
				SessionObjectWidgetRef->Setup(CurrentResult);
				ServerList->AddChild(SessionObjectWidgetRef);
			}		
		}
	}			
}