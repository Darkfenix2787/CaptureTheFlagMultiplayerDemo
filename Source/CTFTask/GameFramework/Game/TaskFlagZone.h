// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaskFlagZone.generated.h"

/**
 *  Zone that handle the score and base for the flag an the team
 */
enum class ETeam : uint8;
class USphereComponent;
UCLASS()
class CTFTASK_API ATaskFlagZone : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	USphereComponent* ZoneSphere;
	
public:	
	// Sets default values for this actor's properties
	ATaskFlagZone();

	UPROPERTY(EditAnywhere)
	ETeam Team;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnZoneSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnZoneSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
