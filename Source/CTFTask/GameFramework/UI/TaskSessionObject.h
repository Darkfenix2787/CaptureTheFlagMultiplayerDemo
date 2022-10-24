// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "TaskSessionObject.generated.h"

class UTaskGameInstance;
class UTextBlock;
class UButton;
/**
 * This Class Handle the Session List behaviour
 */
UCLASS()
class CTFTASK_API UTaskSessionObject : public UUserWidget
{
	GENERATED_BODY()
	
public:	
	
	void Setup(FOnlineSessionSearchResult Session);		

protected:

	UPROPERTY(meta=(BindWidget))
	UButton* JoinButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* ServerName;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* SessionPlayers;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* Joining;

	IOnlineSessionPtr SessionInterface;

	FOnlineSessionSearchResult CurrentSession;

	virtual bool Initialize() override;

	void OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);

	UFUNCTION()
	void JoinButtonClicked();
	
};
