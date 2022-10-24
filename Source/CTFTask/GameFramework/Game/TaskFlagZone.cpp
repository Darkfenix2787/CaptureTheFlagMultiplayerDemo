// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFTask/GameFramework/Game/TaskFlagZone.h"

#include "TaskGameModeGameplay.h"
#include "Components/SphereComponent.h"
#include "CTFTask/GameFramework/Actors/TaskFlag.h"
#include "CTFTask/GameFramework/Character/TaskCharacter.h"


/* -----------------------------------------------------------------------------------------------------------------------------------------*/
ATaskFlagZone::ATaskFlagZone()
{ 	
	PrimaryActorTick.bCanEverTick = false;

	ZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneSphere"));
	SetRootComponent(ZoneSphere);
}

/* -----------------------------------------------------------------------------------------------------------------------------------------*/
void ATaskFlagZone::BeginPlay()
{
	Super::BeginPlay();

	ZoneSphere->OnComponentBeginOverlap.AddDynamic(this, &ATaskFlagZone::OnZoneSphereBeginOverlap);
	ZoneSphere->OnComponentEndOverlap.AddDynamic(this, &ATaskFlagZone::OnZoneSphereEndOverlap);
}

/* -----------------------------------------------------------------------------------------------------------------------------------------*/
void ATaskFlagZone::OnZoneSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATaskFlag* OverlappingFlag = Cast<ATaskFlag>(OtherActor);
	if (OverlappingFlag)
	{
		if(OverlappingFlag->GetTeam() != Team)
		{
			ATaskGameModeGameplay* GameMode = GetWorld()->GetAuthGameMode<ATaskGameModeGameplay>();
			if (GameMode)
			{
				GameMode->PlayerCapturedFlag(this);

				ACTFTaskCharacter* CharacterOwner = Cast<ACTFTaskCharacter>(OverlappingFlag->GetOwner());
				if(CharacterOwner && HasAuthority())
				{
					CharacterOwner->DropFlag();
				}

				OverlappingFlag->ResetFlag();
			}			
		}
		else
		{
			OverlappingFlag->IsInZone(true);
		}
	}
}

/* -----------------------------------------------------------------------------------------------------------------------------------------*/
void ATaskFlagZone::OnZoneSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATaskFlag* OverlappingFlag = Cast<ATaskFlag>(OtherActor);
	if (OverlappingFlag)
	{
		if(OverlappingFlag->GetTeam() == Team)
		{
			OverlappingFlag->IsInZone(false);
		}
	}
}
