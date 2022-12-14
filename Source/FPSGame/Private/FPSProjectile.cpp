// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "FPSCharacter.h"

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);	// set up a notification for when this component hits something blocking

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
	//ProjectileMaterialInst = ProjectileMeshComp->CreateAndSetMaterialInstanceDynamic(0);
	//FLinearColor rColor = FLinearColor::MakeRandomColor();
	//ProjectileMaterialInst->SetVectorParameterValue("BulletColor", rColor);
}


void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	//TArray<UStaticMeshComponent> StaticComps;
	//AActor* gunMesh = GetInstigator()->FindComponentByClass<USkeletalMeshComponent>();
	//GetComponents** <UStaticMeshComponent>** (StaticComps);
	/*gunMesh = GetComponentsByTag(GetInstigator(), "GunMesh")(0);*/

	//ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//USkeletalMeshComponent* playerGunMesh = fpsPlayer->GetComponentsByTag(fpsPlayer, "GunMesh");

	//ProjectileMaterialInst = UMaterialInstanceDynamic::Create(playerGunMesh->GetMaterial(0), this);
	//ProjectileMeshComp->SetMaterial(0, ProjectileMaterialInst);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFPSProjectile::Explode, 3.0f, false);
}

void AFPSProjectile::Explode()
{
	// Allow BP to trigger additional logic
	BlueprintExplode();

	Destroy();
}


void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics object
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		//float RandomIntensity = FMath::RandRange(50.0f, 100.0f);

		//OtherComp->AddImpulseAtLocation(GetVelocity() * RandomIntensity, GetActorLocation());

		//FVector Scale = OtherComp->GetComponentScale();
		//Scale *= 0.8f;

		//if (Scale.GetMin() < 0.5f)
		//{
		//	OtherActor->Destroy();
		//}
		//else
		//{
		//	OtherComp->SetWorldScale3D(Scale);
		//}

		//UMaterialInstanceDynamic* MatInst = OtherComp->CreateDynamicMaterialInstance(0);
		//if (MatInst)
		//{
		//	FLinearColor NewColor = FLinearColor::MakeRandomColor();

		//	MatInst->SetVectorParameterValue("Color", NewColor);
		//}

		OtherActor->Destroy();
	}

	Explode();
}