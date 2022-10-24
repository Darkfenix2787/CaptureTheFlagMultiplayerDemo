// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "TaskPlayerStart.generated.h"

enum class ETeam : uint8;

/**
 *	Class to handle the character by team
 */
UCLASS()
class CTFTASK_API ATaskPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	ETeam Team;
};
