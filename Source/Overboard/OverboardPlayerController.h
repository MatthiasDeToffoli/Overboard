// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OverboardPlayerController.generated.h"
#include "OverboardPlayer.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API AOverboardPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	AOverboardPlayer* GetPlayer();
};
