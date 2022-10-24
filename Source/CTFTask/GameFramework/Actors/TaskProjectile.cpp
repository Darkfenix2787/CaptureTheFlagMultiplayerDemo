// Copyright Epic Games, Inc. All Rights Reserved.

#include "TaskProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "CTFTask/GameFramework/Character/TaskCharacter.h"
#include "Kismet/GameplayStatics.h"

/* ----------------------------------------------------------------------------------------------------------------------------------------------*/
ACTFTaskProjectile::ACTFTaskProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);

	if(HasAuthority())
	{
		CollisionComp->OnComponentHit.AddDynamic(this, &ACTFTaskProjectile::OnHit);		// set up a notification for when this component hits something blocking		
	}

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;	

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	Damage = 10.f;
}

/* ----------------------------------------------------------------------------------------------------------------------------------------------*/
void ACTFTaskProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		if(OwnerCharacter)
		{
			AController* OwnerController = OwnerCharacter->Controller;
			if(OwnerController)
			{
				UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
			}
		}
	}
	
	// Only add impulse and destroy projectile if we hit a physics
	if (OtherComp->IsSimulatingPhysics())
	{	
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());		
	}

	Destroy();
}