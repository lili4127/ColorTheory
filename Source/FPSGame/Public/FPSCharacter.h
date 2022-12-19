// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AFPSProjectile;
class USoundBase;
class UAnimSequence;
class UParticleSystem;
class AFPSBombActor;

DECLARE_DELEGATE(FOnColorChangeDelegate);
UCLASS()
class AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	/** Pawn mesh: 1st person view  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* Mesh1PComponent;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UMaterialInstanceDynamic* GunMaterialInst;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	FLinearColor OrangeColor;
	FLinearColor PurpleColor;
	FLinearColor GreenColor;
	FLinearColor CurrentColor;
	int32 ColorType = 0;

public:
	AFPSCharacter();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AFPSProjectile> ProjectileClass;

	/** Bomb class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Bombs")
	TSubclassOf<AFPSBombActor> BombClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UParticleSystem* MuzzleFlash;

protected:
	
	/** Fires a projectile. */
	void Fire();

	/** Spawn a bomb */
	void SpawnBomb();

	/** Gun select */
	void OrangeGun();

	void PurpleGun();

	void GreenGun();

	/** Change gun and bullet color */
	void ChangeGun(int x);

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }

	FLinearColor GetCurrentColor();
	int32 GetColorType();

};

