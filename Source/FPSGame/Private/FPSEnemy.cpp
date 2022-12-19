// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSEnemy.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "FPSGameMode.h"

// Called when the game starts or when spawned
void AFPSEnemy::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AFPSEnemy::HandleDestruction()
{
	GameMode->ActorDied(this);
	Destroy();
	//SetActorHiddenInGame(true);
	//SetActorTickEnabled(false);
}

int32 AFPSEnemy::GetEnemyColor()
{
	return enemyColor;
}



