// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "TaskGameInstance.generated.h"

/**
 * This Class will manage the online behaviour
 */
UCLASS()
class CTFTASK_API UTaskGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	// Properties
	
	IOnlineSessionPtr SessionInterface;
	
	TSharedPtr<FOnlineSessionSearch> SessionSearch;	
	
	FName SessionDefaultName = TEXT("SessionName");

	int32 PublicConnections;


	// Functions
	
	void Host(int32 NumPublicConnections);
	
	void Join(const FOnlineSessionSearchResult& SessionToJoin) const;

	UFUNCTION(BlueprintCallable)
	void Search();
	
	virtual void Init() override;
	
	void SetSession(int32 NumPublicConnections) const;
	
	void DestroySession (FName SessionName) const;
	
	void OnSessionDestroyed(FName SessionName, bool Success);
	
};
