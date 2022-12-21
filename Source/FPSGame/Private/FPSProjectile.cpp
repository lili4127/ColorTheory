// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "FPSCharacter.h"
#include "FPSEnemy.h"
#include "Materials/MaterialInstanceDynamic.h"

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSProjectile::OverlapBegin);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 4000.f;
	ProjectileMovement->MaxSpeed = 4000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0;

	// Create a projectile mesh component
	ProjectileMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMeshComp->SetupAttachment(CollisionComp);

	CharacterActorReference = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMaterialInst = ProjectileMeshComp->CreateAndSetMaterialInstanceDynamic(0);
	ProjectileMaterialInst->SetVectorParameterValue("BulletColor", CharacterActorReference->GetCurrentColor());

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFPSProjectile::Explode, 3.0f, false);
}

void AFPSProjectile::Explode()
{
	// Allow BP to trigger additional logic
	Destroy();
}

void AFPSProjectile::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		AFPSEnemy* Enemy = Cast<AFPSEnemy>(OtherActor);
		if (Enemy)
		{
			if (Enemy->GetEnemyKillColor().Equals(ProjectileMaterialInst->K2_GetVectorParameterValue("BulletColor")))
			{
				Enemy->HandleDestruction();
			}

			else
			{
				BlueprintExplode();
			}
		}
	}

	Explode();
}
