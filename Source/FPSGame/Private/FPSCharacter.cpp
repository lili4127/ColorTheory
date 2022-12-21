// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequence.h"
#include "FPSBombActor.h"

AFPSCharacter::AFPSCharacter()
{
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));
	Mesh1PComponent->SetRelativeLocation(FVector(0, 0, -160.0f));

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");

	OrangeColor = FLinearColor(0.723055f, 0.095308f, 0.011612f, 1);
	PurpleColor = FLinearColor(0.099899f, 0.019382f, 0.291771f, 1);
	GreenColor = FLinearColor(0, 1, 0, 1);
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
	//PlayerInputComponent->BindAction("SpawnBomb", IE_Pressed, this, &AFPSCharacter::SpawnBomb);
	PlayerInputComponent->BindAction("OrangeGun", IE_Pressed, this, &AFPSCharacter::OrangeGun);
	PlayerInputComponent->BindAction("PurpleGun", IE_Pressed, this, &AFPSCharacter::PurpleGun);
	PlayerInputComponent->BindAction("GreenGun", IE_Pressed, this, &AFPSCharacter::GreenGun);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	GunMaterialInst = UMaterialInstanceDynamic::Create(GunMeshComponent->GetMaterial(0), this);
	GunMaterialInst->SetVectorParameterValue("BodyColor", OrangeColor);
	GunMeshComponent->SetMaterial(0, GunMaterialInst);
	CurrentColor = OrangeColor;
}

FLinearColor AFPSCharacter::GetCurrentColor()
{
	return CurrentColor;
}

void AFPSCharacter::Fire()
{
	// try and fire a projectile
	if (ProjectileClass)
	{
		// Grabs location from the mesh that must have a socket called "Muzzle" in his skeleton
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		// Use controller rotation which is our view direction in first person
		FRotator MuzzleRotation = GetControlRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = this;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), 0.25f);
	}

	// try and play a firing animation if specified
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}

	// Play Muzzle FX
	//UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMeshComponent, "Muzzle");
}

void AFPSCharacter::SpawnBomb()
{
	// try and fire a projectile
	if (BombClass)
	{
		// Grabs location from the mesh that must have a socket called "Muzzle" in his skeleton
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		// Use controller rotation which is our view direction in first person
		FRotator MuzzleRotation = GetControlRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = this;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AFPSBombActor>(BombClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}

	// Play Muzzle FX
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMeshComponent, "Muzzle");
}

void AFPSCharacter::OrangeGun()
{
	ChangeGun(0);
}

void AFPSCharacter::PurpleGun()
{
	ChangeGun(1);
}

void AFPSCharacter::GreenGun()
{
	ChangeGun(2);
}

void AFPSCharacter::ChangeGun(int x)
{
	switch (x)
	{
	case 0:
		//UE_LOG(LogTemp, Warning, TEXT("Orange"));
		GunMaterialInst->SetVectorParameterValue("BodyColor", OrangeColor);
		CurrentColor = OrangeColor;
		break;
	case 1:
		//UE_LOG(LogTemp, Warning, TEXT("Purple"));
		GunMaterialInst->SetVectorParameterValue("BodyColor", PurpleColor);
		CurrentColor = PurpleColor;
		break;
	case 2:
		//UE_LOG(LogTemp, Warning, TEXT("Green"));
		GunMaterialInst->SetVectorParameterValue("BodyColor", GreenColor);
		CurrentColor = GreenColor;
		break;
	default:
		break;
	}
}

void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}