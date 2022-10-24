// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "TaskMenu.generated.h"

class UTaskServerList;
class UTaskGameInstance;
class UWidgetSwitcher;
class UButton;
/**
 * Widget That controls the session interactions
 */
UCLASS()
class CTFTASK_API UTaskMenu : public UUserWidget
{
	GENERATED_BODY()

	
public:
	
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString LobbyPath = FString(TEXT("/Game/Maps/Looby")));

protected:

	UPROPERTY(BlueprintReadWrite)
	FString PathToLobby{TEXT("")};

	/** Number of connections/players */
	int32 NumPublicConnections;

	/** GameInstance Reference */
	UTaskGameInstance* GameInstance;

	/** Session Interface Reference */
	IOnlineSessionPtr SessionInterface;
	
	virtual bool Initialize() override;
	
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

private:	

	UPROPERTY(meta=(BindWidget))
	UButton* CreateGameButton;	

	UPROPERTY(meta=(BindWidget))
	UButton* BrowseGamesButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* ContextMenuSwitcher;

	UPROPERTY(meta=(BindWidget))
	UTaskServerList* ServerList;
	
	void MenuTearDown();

	UFUNCTION()
	void CreateGameButtonClicked();

	UFUNCTION()
	void BrowseGamesButtonClicked();
	
	UFUNCTION()
	void QuitGameButtonClicked();

	void OnSessionCreated(FName SessionName, bool Success) const;

	void OnSessionFound(bool Success) const;
	
};
