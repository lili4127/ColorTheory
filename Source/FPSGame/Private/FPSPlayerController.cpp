// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"
#include "GameFramework/Character.h"

void AFPSPlayerController::SetPlayerEnabledState(bool playerEnabled)
{
	if (playerEnabled)
	{
		GetCharacter()->EnableInput(this);
	}

	else
	{
		GetCharacter()->DisableInput(this);
	}
}
