// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CTFTask/GameFramework/Game/Team.h"
#include "GameFramework/Character.h"
#include "TaskCharacter.generated.h"

class USphereComponent;
class UCameraComponent;
class ATaskFlag;
class ATaskPlayerController;
class UInputComponent;
class ATaskPlayerState;

UCLASS(config=Game)
class ACTFTaskCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Gun mesh: 1st person view (seen only by others) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* OP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USphereComponent* OverlapComponent;


protected:
	
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

public:

	/** Constructor */
	ACTFTaskCharacter();
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Aim Rotation Offset */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Replicated, Category=Camera)
	FRotator AimRotation;

	/** Socket Name where the Gun will be attached */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FName GunSocketName;

	/** Socket Name where the Flag will be attached */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FName FlagSocketName;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ACTFTaskProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire (arms; seen only by self)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;	

	/** AnimMontage to play each time the character Dies (Body; seen only by others) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimationAsset* BodyDeathAnimation;
	
	/** Material for the red team character mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UMaterialInstance* RedMaterial;

	/** Material for the blue team character mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UMaterialInstance* BlueMaterial;

	UPROPERTY()
	ATaskPlayerState* CharacterPlayerState;	

protected:

	UPROPERTY()
	ATaskPlayerController* PlayerController;

	/** Player Max health */
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth;

	/** Player current health */
	UPROPERTY(EditDefaultsOnly,ReplicatedUsing = OnRep_Health)
	float Health;

	UFUNCTION()
	void OnRep_Health();

	/** Flag to know when the player was eliminated */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_bWasEliminated)
	uint8 bWasEliminated : 1;

	UFUNCTION()
	void OnRep_bWasEliminated();
	
	/** RespawnTimerHandle  */
	UPROPERTY(Transient)
	FTimerHandle RespawnTimerHandle;

	// time for respawn when a player dies
	UPROPERTY(EditDefaultsOnly)
	float RespawnTime;

	/** Overlapped flag reference */
	UPROPERTY(ReplicatedUsing = OnRep_OverlappedFlag)
	ATaskFlag* OverlappedFlag;

	UFUNCTION()
	void OnRep_OverlappedFlag(ATaskFlag* LastOverlappedFlag);

	/** Reference to the equipped flag */
	UPROPERTY(Replicated)
	ATaskFlag* EquippedFlag;

	/** Flag to know id the player is holding the flag */
	UPROPERTY(ReplicatedUsing = OnRep_bIsHoldingFlag)
	uint8 bIsHoldingFlag : 1;

	UFUNCTION()
	void OnRep_bIsHoldingFlag();

	/** Fires a projectile. */
	UFUNCTION()
	void FireWeapon();

	/** Equips the flag */
	UFUNCTION()
	void PressEquipButton();
	
	/** Server Equips the flag */
    UFUNCTION(Server,Reliable,WithValidation)
    void Server_PressEquipButton();

	// Equips the flag
	UFUNCTION()
	void EquipFlag(ATaskFlag* FlagToEquip);
	
	/** Fires a projectile. */
	UFUNCTION(Server, WithValidation, Reliable)
	void Server_SpawnProjectile();	

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);	

	/** APawn interface */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;		

	
	virtual void GetLifetimeReplicatedProps (TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult );

	/** Server Function to sync the aim rotation */
	UFUNCTION(Server, WithValidation, Reliable)
	void Server_SyncAimRotation ();

	/** Multicast Function to sync the aim rotation */
	UFUNCTION(Server, Reliable)
	void Multicast_SyncAimRotation ();

	/** Function to sync the aim rotation */
	void SyncAimRotation();

	/** Function to update the health in the HUD player */
	void UpdateHUDHealth();

	/** Function to update the Flag Image in the HUD player */
	void UpdateHUDFlag();

	/** Function to update the Dead Hud */
	void UpdateHUDDead(const bool bIsDead);

	/** is used to ensure that the relevant properties are valid */
	void initialize();	

	UFUNCTION()
	void SetSpawnTeam();
	
public:
	
	/** Returns Mesh1P subobject **/
	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// Sets the Character Color Based on its team
	void SetTeamColor(ETeam Team) const;

	UFUNCTION()
	void ReceiveDamage (AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	
	void Eliminated();

	void RespawnCharacter();

	// bWasEliminated Getter
	UFUNCTION(BlueprintCallable)
	bool WasEliminated () const { return bWasEliminated; }

	// Health Getter
	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	// MaxHealth Getter
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const { return MaxHealth; }

	// Set the flag that the character is overlapping
	void SetOverlappingFlag( ATaskFlag* Flag );

	// Drops the equipped flag
	UFUNCTION()
	void DropFlag();

	// Gets the character Team
	ETeam GetTeam();

	void IsHoldingFlag (const bool bNewValue ) { bIsHoldingFlag = bNewValue ;}
};