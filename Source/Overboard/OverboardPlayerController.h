// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OverboardPlayer.h"
#include "OverboardPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class OVERBOARD_API AOverboardPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private :
	/**
	* All enemies in player view
	*/
	TArray<AActor*> enemiesInView;

public:
	/**
	* Get the current player on type AOverboardPlayer
	*/
	AOverboardPlayer* GetPlayer();

	/**
	* Update enemies array in player view datas
	*/
	void UpdateEnemiesInView(TArray<AActor*> pEnemies);
};
