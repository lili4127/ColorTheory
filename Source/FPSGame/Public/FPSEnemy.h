// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FPSEnemy.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class FPSGAME_API AFPSEnemy : public APawn
{
	GENERATED_BODY()

public:
	void HandleDestruction();
	int32 GetEnemyColor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	class AFPSGameMode* GameMode;

	UPROPERTY(BlueprintReadWrite, Category = Enemy)
	int32 enemyColor = 0;
};
