// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "FPSPlayerController.h"
#include "FPSEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

void AFPSGameMode::ActorDied(AActor* DeadActor)
{
	if (AFPSEnemy* Enemy = Cast<AFPSEnemy>(DeadActor))
	{
		Enemies--;
		//UE_LOG(LogTemp, Warning, TEXT("Enemies: %d"), Enemies);

		if (Enemies <= 0)
		{
			GameOver(true);
		}
	}
}

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UGameplayStatics::GetCurrentLevelName(GetWorld()).Equals("MainLevel"))
	{
		HandleGameStart();
	}
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

	Enemies = GetEnemyCount();
	//UE_LOG(LogTemp, Warning, TEXT("Enemies: %d"), Enemies);
}

int32 AFPSGameMode::GetEnemyCount()
{
	TArray<AActor*> EnemiesArr;
	UGameplayStatics::GetAllActorsOfClass(this, AFPSEnemy::StaticClass(), EnemiesArr);
	return EnemiesArr.Num();
}
