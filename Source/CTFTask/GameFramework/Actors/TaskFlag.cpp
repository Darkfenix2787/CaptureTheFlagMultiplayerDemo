// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskFlag.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "CTFTask/GameFramework/Character/TaskCharacter.h"
#include "Net/UnrealNetwork.h"

/* -----------------------------------------------------------------------------------------------------------*/
ATaskFlag::ATaskFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
	SetRootComponent(FlagMesh);
	FlagMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(FlagMesh);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(FlagMesh);

	bIsEquipped = false;
	bIsInZone = true;	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskFlag::BeginPlay()
{
	Super::BeginPlay();

	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ATaskFlag::OnAreaSphereBeginOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ATaskFlag::OnAreaSphereEndOverlap);

	if(PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}

	InitialTransform = GetActorTransform();	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskFlag::OnAreaSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACTFTaskCharacter* CharacterRef = Cast<ACTFTaskCharacter>(OtherActor);
	if(CharacterRef)
	{
		if(CharacterRef->GetTeam() != Team && bIsInZone)
		{
			CharacterRef->SetOverlappingFlag(this);
		}
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskFlag::OnAreaSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACTFTaskCharacter* CharacterRef = Cast<ACTFTaskCharacter>(OtherActor);
	if(CharacterRef)
	{
		if( CharacterRef->GetTeam() != Team )
		{
			CharacterRef->SetOverlappingFlag(nullptr);
		}
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskFlag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATaskFlag, bIsEquipped);
	DOREPLIFETIME(ATaskFlag, bIsInZone);
	DOREPLIFETIME(ATaskFlag, InitialTransform);	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskFlag::IsEquipped(const bool bNewState)
{
	bIsEquipped = bNewState;
	ShowPickupWidget(!bIsEquipped);
	AreaSphere->SetCollisionEnabled( bIsEquipped ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryOnly );
	FlagMesh->SetOwnerNoSee(bIsEquipped);

	FlagMesh->SetSimulatePhysics(!bNewState);
	FlagMesh->SetEnableGravity(!bNewState);
	FlagMesh->SetCollisionEnabled(bNewState ? ECollisionEnabled::QueryOnly : ECollisionEnabled::QueryAndPhysics);
	FlagMesh->SetCollisionResponseToAllChannels( bNewState ? ECR_Overlap : ECR_Block);
	FlagMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	FlagMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	if(!bIsEquipped)
	{
		SetOwner(nullptr);
		ShowPickupWidget(bIsEquipped);
		ShowPickupWidget(bIsEquipped);
		const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskFlag::OnRep_bIsEquipped()
{
	ShowPickupWidget(!bIsEquipped);
	AreaSphere->SetCollisionEnabled( bIsEquipped ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryOnly);
	FlagMesh->SetOwnerNoSee(bIsEquipped);

	FlagMesh->SetSimulatePhysics(!bIsEquipped);
	FlagMesh->SetEnableGravity(!bIsEquipped);
	FlagMesh->SetCollisionEnabled(bIsEquipped ? ECollisionEnabled::QueryOnly : ECollisionEnabled::QueryAndPhysics);
	FlagMesh->SetCollisionResponseToAllChannels( bIsEquipped ? ECR_Overlap : ECR_Block);
	FlagMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	FlagMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	

	if(!bIsEquipped)
	{
		SetOwner(nullptr);
		ShowPickupWidget(bIsEquipped);
		const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
		FlagMesh->DetachFromComponent(DetachRules);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskFlag::ShowPickupWidget(bool bShowWidget) const
{
	if(PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ATaskFlag::ResetFlag()
{
	SetActorTransform(InitialTransform);	
}