// Fill out your copyright notice in the Description page of Project Settings.


#include "OverboardPlayerController.h"

AOverboardPlayer* AOverboardPlayerController::GetPlayer()
{
	APawn* lPawn = GetPawn();

	if (lPawn)
	{
		return Cast<AOverboardPlayer>(lPawn);
	}

	return nullptr;
}