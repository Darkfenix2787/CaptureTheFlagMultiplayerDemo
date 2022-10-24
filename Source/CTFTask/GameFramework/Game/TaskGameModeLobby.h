// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskGameMode.h"
#include "TaskGameModeLobby.generated.h"


class UTaskGameInstance;
/**
 * A Game Mode for the lobby
 */
UCLASS()
class CTFTASK_API ATaskGameModeLobby : public ATaskGameMode
{
	GENERATED_BODY()

public:

	virtual void PostLogin(APlayerController* NewPlayer) override;	
	
	void LogOutPlayer();
	
	FName SessionDefaultName = TEXT("SessionName");

	FTimerHandle WaitPlayersHandle;

	/** Reference to gameInstance */
	UTaskGameInstance* GameInstanceRef;

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FString PathToGame{TEXT("")};

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FString PathToMainMenu{TEXT("")};	
};
