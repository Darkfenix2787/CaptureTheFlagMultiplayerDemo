// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TaskStartUp.generated.h"

/**
 * A Press Any Button Widget
 */
UCLASS()
class CTFTASK_API UTaskStartUp : public UUserWidget
{
	GENERATED_BODY()

	
public:

	UFUNCTION(BlueprintCallable)
	void StartUpSetup(const FName MainMenuMapName = FName(TEXT("MainMenu")));

	UFUNCTION()
	void GoToMainMenu() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MainMenuLevelName;
	
};
