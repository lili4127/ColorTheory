// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "FPSCharacter.h"
#include "Materials/MaterialInstanceDynamic.h"

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
	ProjectileMeshComp->SetupAttachment(CollisionComp);

	CharacterActorReference = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (CharacterActorReference)
	{
		CharacterActorReference->OnColorChange.BindUObject(this, &AFPSProjectile::ColorChangeEventFunction);
	}
}


void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMaterialInst = ProjectileMeshComp->CreateAndSetMaterialInstanceDynamic(0);
	ProjectileMaterialInst->SetVectorParameterValue("BulletColor", CharacterActorReference->GetCurrentColor());

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFPSProjectile::Explode, 3.0f, false);
}


void AFPSProjectile::ColorChangeEventFunction()
{
	CurrentColor = FLinearColor::MakeRandomColor();
	UE_LOG(LogTemp, Warning, TEXT("Event working!"));
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