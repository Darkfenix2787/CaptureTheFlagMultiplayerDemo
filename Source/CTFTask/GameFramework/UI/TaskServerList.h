// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TaskServerList.generated.h"

class UTaskGameInstance;
class UTaskSessionObject;
class FOnlineSessionSearch;
class UScrollBox;
class UTextBlock;

/**
 * Widget that contains the active server list
 */
UCLASS()
class CTFTASK_API UTaskServerList : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* Servers;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UScrollBox* ServerList;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTaskSessionObject> SessionObjectWidget;

public:
	
	void PopulateListServer(TArray<FOnlineSessionSearchResult> SearchResults);	
};
