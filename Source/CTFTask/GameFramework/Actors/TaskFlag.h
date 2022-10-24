// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaskFlag.generated.h"


enum class ETeam : uint8;
class UWidgetComponent;
class USphereComponent;

// Clas for handle the Flag Behaviour
UCLASS()
class CTFTASK_API ATaskFlag : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, Category = "Flag Properties")
	UStaticMeshComponent* FlagMesh;

	UPROPERTY(VisibleAnywhere, Category = "Flag Properties")
	USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Flag Properties")
	UWidgetComponent* PickupWidget;

	UPROPERTY(ReplicatedUsing=OnRep_bIsEquipped, VisibleAnywhere, Category = "Flag Properties")
	uint8 bIsEquipped : 1;

	UPROPERTY(Replicated)
	uint8 bIsInZone : 1;

	UPROPERTY(EditAnywhere)
	ETeam Team;

	UPROPERTY(Replicated)
	FTransform InitialTransform;

	UFUNCTION()
	void OnRep_bIsEquipped();

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Area Sphere Begin Overlap Function */
	UFUNCTION()
	virtual void OnAreaSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult );

	/** Area Sphere End Overlap Function */
	UFUNCTION()
	virtual void OnAreaSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/* Replication properties conditions */
	virtual void GetLifetimeReplicatedProps (TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	/** Sets default values for this actor's properties */
	ATaskFlag();	

	/** Shows the PickUp widget */
	void ShowPickupWidget(bool bShowWidget) const;

	/** Hendle the behaviour when the flag is Equipped/Unequipped */ 
	void IsEquipped(const bool bNewState);

	/** Area Sphere Getter */
	USphereComponent* GetAreaSphere() const { return AreaSphere; }

	/** Team Getter */
	ETeam GetTeam() const { return Team; }

	/** Function to reset the flag position */
	void ResetFlag();

	/** bIsInZone Setter */
	void IsInZone(bool bNewState) { bIsInZone = bNewState; }	

	/** InitialTransform Getter */
	UFUNCTION(BlueprintCallable)
	FTransform GetInitialTransform(){ return InitialTransform; }
};
