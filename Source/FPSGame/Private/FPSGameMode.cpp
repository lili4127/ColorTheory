// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FPSPlayerController.h"

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
}

void AFPSGameMode::HandleGameStart()
{
	FPSCharacter = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	FPSPlayerController = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (FPSPlayerController)
	{
		FPSPlayerController->SetPlayerEnabledState(false);
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			FPSPlayerController,
			&AFPSPlayerController::SetPlayerEnabledState,
			true
		);
		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, StartDelay, false);
	}
}
