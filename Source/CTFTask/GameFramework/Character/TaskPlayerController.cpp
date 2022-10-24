// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFTask/GameFramework/Character/TaskPlayerController.h"

#include "TaskCharacter.h"
#include "CTFTask/GameFramework/UI/TaskHUD.h"
#include "GameFramework/CharacterMovementComponent.h"

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TaskHUD = Cast<ATaskHUD>(GetHUD());
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ACTFTaskCharacter* CharacterRef = Cast<ACTFTaskCharacter>(InPawn);
	if(CharacterRef)
	{	
		SetHUDHealth(CharacterRef->GetHealth(), CharacterRef->GetMaxHealth());
		SetHUDFlagVisibility(false);
		UpdateHUDDead(false);
		CharacterRef->CharacterPlayerState = nullptr;
	}	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskPlayerController::SetHUDHealth(const float Health, const float MaxHealth)
{
	TaskHUD = TaskHUD == nullptr ? Cast<ATaskHUD>(GetHUD()) : TaskHUD;
	if(TaskHUD && MaxHealth != 0.f)
	{
		const float HealthPercent = Health / MaxHealth;
		TaskHUD->SetHealth(HealthPercent);
	}	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskPlayerController::SetHUDRedTeamScore(int32 RedScore)
{
	TaskHUD = TaskHUD == nullptr ? Cast<ATaskHUD>(GetHUD()) : TaskHUD;	
	if (TaskHUD)
	{
		TaskHUD->SetRedTeamScore(RedScore);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskPlayerController::SetHUDBlueTeamScore(int32 BlueScore)
{
	TaskHUD = TaskHUD == nullptr ? Cast<ATaskHUD>(GetHUD()) : TaskHUD;	
	if (TaskHUD)
	{
		TaskHUD->SetBlueTeamScore(BlueScore);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskPlayerController::SetHUDFlagVisibility(const bool bIsVisible)
{
	TaskHUD = TaskHUD == nullptr ? Cast<ATaskHUD>(GetHUD()) : TaskHUD;
	if(TaskHUD)
	{
		TaskHUD->SetFlagVisibility(bIsVisible);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskPlayerController::SetHUDTimer(float Time)
{
	TaskHUD = TaskHUD == nullptr ? Cast<ATaskHUD>(GetHUD()) : TaskHUD;
	if (TaskHUD)
	{
		TaskHUD->SetTimer(Time);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskPlayerController::SetHUDAnnouncement(const FString AnnouncementText)
{
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);
	DisableInput(this);
	
	TaskHUD = TaskHUD == nullptr ? Cast<ATaskHUD>(GetHUD()) : TaskHUD;
	if (TaskHUD)
	{
		TaskHUD->SetAnnouncement(AnnouncementText);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskPlayerController::UpdateHUDDead(const bool bIsDead)
{
	TaskHUD = TaskHUD == nullptr ? Cast<ATaskHUD>(GetHUD()) : TaskHUD;
	if (TaskHUD)
	{
		TaskHUD->SetDead(bIsDead);
	}
}
