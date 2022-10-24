// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TaskPlayerController.generated.h"

class ATaskPlayerState;
class ACTFTaskCharacter;
class ATaskHUD;
/**
 * Character Player Controller
 */
UCLASS()
class CTFTASK_API ATaskPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;	
	
	UPROPERTY()
	ATaskHUD * TaskHUD;	

public:

	// UI Setters
	void SetHUDHealth(const float Health, const float MaxHealth);

	void SetHUDFlagVisibility(const bool bIsVisible);

	void SetHUDRedTeamScore(int32 RedScore);
	
	void SetHUDBlueTeamScore(int32 BlueScore);

	void SetHUDTimer(float Time);
	
	void SetHUDAnnouncement(const FString AnnouncementText);

	void UpdateHUDDead(const bool bIsDead);
};