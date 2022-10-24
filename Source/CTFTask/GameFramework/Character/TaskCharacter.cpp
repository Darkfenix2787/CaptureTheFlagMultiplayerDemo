// Copyright Epic Games, Inc. All Rights Reserved.

#include "TaskCharacter.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "TaskPlayerController.h"
#include "Components/SphereComponent.h"
#include "CTFTask/GameFramework/Actors/TaskProjectile.h"
#include "CTFTask/GameFramework/Game/TaskGameModeGameplay.h"
#include "CTFTask/GameFramework/Game/TaskPlayerState.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "CTFTask/GameFramework/Actors/TaskFlag.h"
#include "CTFTask/GameFramework/Game/TaskPlayerStart.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ACTFTaskCharacter
/* -----------------------------------------------------------------------------------------------------------*/
ACTFTaskCharacter::ACTFTaskCharacter()
{
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));	

	// Create a gun mesh component to '1st person' view (when controlling this pawn)
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;	
	FP_Gun->SetupAttachment(Mesh1P);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.
	
	// Create a gun mesh component to '1st person' view (when not controlling this pawn)
	OP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("OP_Gun"));
	OP_Gun->SetOwnerNoSee(true);		// only the others players will see this mesh
	OP_Gun->bCastDynamicShadow = false;
	OP_Gun->CastShadow = false;	
	OP_Gun->SetupAttachment(GetMesh());

	OverlapComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComponent"));
	OverlapComponent->SetupAttachment(GetMesh());
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	GunSocketName = "Gun_Socket";
	FlagSocketName = "SK_Flag";	
	Health = 100.f;
	MaxHealth = 100.f;	
	RespawnTime = 1.5f;
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	//Attach Gun mesh component to the Body Skeleton only if the character is not locally controlled
	if(!IsLocallyControlled())
	{
		OP_Gun->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), GunSocketName);		
	}
	
	UpdateHUDHealth();
	UpdateHUDDead(false);
	initialize();
		
	if(HasAuthority())		
	{
		OnTakeAnyDamage.AddDynamic(this,&ACTFTaskCharacter::ReceiveDamage);
		OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ACTFTaskCharacter::OnCharacterBeginOverlap);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);	
	initialize();
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::initialize()
{
	if(CharacterPlayerState == nullptr || GetTeam() == ETeam::ET_NoTeam)
	{
		CharacterPlayerState = GetPlayerState<ATaskPlayerState>();
		if(CharacterPlayerState)
		{
			if(GetTeam() == ETeam::ET_NoTeam)
			{
				ATaskGameModeGameplay* GameModeRef = GetWorld()->GetAuthGameMode<ATaskGameModeGameplay>();
				if(GameModeRef)
				{
					GameModeRef->SetTeam();
				}
			}
				
			SetTeamColor(CharacterPlayerState->GetTeam());
			SetSpawnTeam();
		}		
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACTFTaskCharacter::FireWeapon);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ACTFTaskCharacter::PressEquipButton);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ACTFTaskCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACTFTaskCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACTFTaskCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACTFTaskCharacter::LookUpAtRate);
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	
	if(GetLocalRole() == ENetRole::ROLE_Authority)
	{
		Multicast_SyncAimRotation();
	}

	Server_SyncAimRotation();
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTFTaskCharacter, AimRotation);
	DOREPLIFETIME(ACTFTaskCharacter, Health);
	DOREPLIFETIME_CONDITION(ACTFTaskCharacter, OverlappedFlag, COND_OwnerOnly);
	DOREPLIFETIME(ACTFTaskCharacter, EquippedFlag);
	DOREPLIFETIME(ACTFTaskCharacter, bIsHoldingFlag);
	DOREPLIFETIME(ACTFTaskCharacter, bWasEliminated);	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATaskFlag* OverlappingFlag = Cast<ATaskFlag>(OtherActor);
	if (OverlappingFlag)
	{
		if(OverlappingFlag->GetTeam() == GetTeam())
		{
			OverlappingFlag->ResetFlag();
		}
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::FireWeapon()
{
	Server_SpawnProjectile();

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	
	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::PressEquipButton()
{
	if(bIsHoldingFlag)
	{
		return;
	}
	
	if(OverlappedFlag)
	{
		if(HasAuthority())
		{
			EquipFlag(OverlappedFlag);			
		}
		else
		{
			Server_PressEquipButton();
		}		
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::Server_PressEquipButton_Implementation()
{
	if(OverlappedFlag)
	{
		EquipFlag(OverlappedFlag);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
bool ACTFTaskCharacter::Server_PressEquipButton_Validate()
{
	return IsValid(OverlappedFlag); 
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::EquipFlag(ATaskFlag* FlagToEquip)
{
	if(FlagToEquip)
	{
		EquippedFlag = FlagToEquip;
		EquippedFlag->SetOwner(this);
		EquippedFlag->IsEquipped(true);
		bIsHoldingFlag = true;
		UpdateHUDFlag();
		
		const USkeletalMeshSocket* FlagSocket = GetMesh()->GetSocketByName(FlagSocketName);
		if(FlagSocket)
		{
			FlagSocket->AttachActor(EquippedFlag,GetMesh());
		}				
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::DropFlag()
{
	if(EquippedFlag)
	{
		bIsHoldingFlag = false;
		EquippedFlag->IsEquipped(false);		
		EquippedFlag = nullptr;
		UpdateHUDFlag();
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::SetOverlappingFlag(ATaskFlag* Flag)
{
	if(OverlappedFlag)
	{
		OverlappedFlag->ShowPickupWidget(false);
	}
	
	OverlappedFlag = Flag;
	if(IsLocallyControlled())
	{
		if(OverlappedFlag)
		{
			OverlappedFlag->ShowPickupWidget(true);
		}
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
ETeam ACTFTaskCharacter::GetTeam()
{
	if(CharacterPlayerState)
	{
		return CharacterPlayerState->GetTeam();			
	}

	return ETeam::ET_NoTeam;
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::OnRep_OverlappedFlag(ATaskFlag* LastOverlappedFlag)
{
	if(OverlappedFlag)
	{
		OverlappedFlag->ShowPickupWidget(true);
	}

	if(LastOverlappedFlag)
	{
		LastOverlappedFlag->ShowPickupWidget(false);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::OnRep_bIsHoldingFlag()
{
	UpdateHUDFlag();
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::Server_SpawnProjectile_Implementation()
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{			
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Owner = this;
			ActorSpawnParams.Instigator = Cast<APawn>(this);

			// spawn the projectile at the muzzle
			World->SpawnActor<ACTFTaskProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);			
		}
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
bool ACTFTaskCharacter::Server_SpawnProjectile_Validate()
{
	return true;
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::OnRep_Health()
{
	UpdateHUDHealth();
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::OnRep_bWasEliminated()
{
	UpdateHUDDead(bWasEliminated);
	if(BodyDeathAnimation && bWasEliminated)
	{
		GetMesh()->PlayAnimation(BodyDeathAnimation,false);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::UpdateHUDHealth()
{
	PlayerController = PlayerController == nullptr ? Cast<ATaskPlayerController>(Controller) : PlayerController;
	if(PlayerController)
	{
		PlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::UpdateHUDFlag()
{
	PlayerController = PlayerController == nullptr ? Cast<ATaskPlayerController>(Controller) : PlayerController;
	if(PlayerController)
	{
		PlayerController->SetHUDFlagVisibility(bIsHoldingFlag);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::UpdateHUDDead(const bool bIsdead)
{
	PlayerController = PlayerController == nullptr ? Cast<ATaskPlayerController>(Controller) : PlayerController;
	if(PlayerController)
	{	
		PlayerController->UpdateHUDDead(bIsdead);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::SetSpawnTeam()
{
	if(HasAuthority() && CharacterPlayerState->GetTeam() != ETeam::ET_NoTeam)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, ATaskPlayerStart::StaticClass(), PlayerStarts);
		TArray<ATaskPlayerStart*> TeamPlayerStarts;
		for(const auto Start :  PlayerStarts)
		{
			ATaskPlayerStart* TeamStart = Cast<ATaskPlayerStart>(Start);
			if(TeamStart && TeamStart->Team == CharacterPlayerState->GetTeam())
			{
				TeamPlayerStarts.Add(TeamStart);
			}
		}

		if(TeamPlayerStarts.Num() > 0)
		{
			const ATaskPlayerStart* ChosenPlayerStart = TeamPlayerStarts[FMath::RandRange(0, TeamPlayerStarts.Num() - 1)];
			SetActorLocationAndRotation(ChosenPlayerStart->GetActorLocation(), ChosenPlayerStart->GetActorRotation());
		}
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::SyncAimRotation()
{
	AimRotation = GetControlRotation();
	if(IsValid(FirstPersonCameraComponent))
	{
		FirstPersonCameraComponent->SetWorldRotation(AimRotation);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::Server_SyncAimRotation_Implementation()
{
	SyncAimRotation();
}

/* -----------------------------------------------------------------------------------------------------------*/
bool ACTFTaskCharacter::Server_SyncAimRotation_Validate()
{
	return true;
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::Multicast_SyncAimRotation_Implementation()
{
	SyncAimRotation();
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::SetTeamColor(ETeam Team) const
{
	switch (Team)
	{
	case ETeam::ET_BlueTeam:
		Mesh1P->SetMaterial(0,BlueMaterial);
		GetMesh()->SetMaterial(0,BlueMaterial);
		break;
	case ETeam::ET_RedTeam:
		Mesh1P->SetMaterial(0,RedMaterial);
		GetMesh()->SetMaterial(0,RedMaterial);
		break;		
	}
	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UpdateHUDHealth();
	
	if (Health == 0.f)
	{
		Eliminated();	
	}	
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::Eliminated()
{
	bWasEliminated = true;
	GetCharacterMovement()->DisableMovement();
	OnTakeAnyDamage.RemoveDynamic(this,&ACTFTaskCharacter::ReceiveDamage);
	UpdateHUDDead(bWasEliminated);	
	DropFlag();
	if(GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle,this,&ACTFTaskCharacter::RespawnCharacter,RespawnTime,false);
	}

	if(BodyDeathAnimation)
	{
		GetMesh()->PlayAnimation(BodyDeathAnimation,false);
	}
}

/* -----------------------------------------------------------------------------------------------------------*/
void ACTFTaskCharacter::RespawnCharacter()
{
	ATaskGameModeGameplay* GameModeRef = GetWorld()->GetAuthGameMode<ATaskGameModeGameplay>();
	if(GameModeRef)
	{
		GameModeRef->RequestRespawn(this,Controller);
	}
}