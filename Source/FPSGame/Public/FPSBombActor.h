// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBombActor.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;
class UParticleSystemComponent;

UCLASS()
class FPSGAME_API AFPSBombActor : public AActor
{
	GENERATED_BODY()

protected:

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PEffects")
	UParticleSystemComponent* BombParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PEffects")
	UParticleSystem* ExplosionFX;

	UFUNCTION()
	void Explode();

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintExplode();

public:

	// Sets default values for this actor's properties
	AFPSBombActor();

	virtual void BeginPlay() override;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
