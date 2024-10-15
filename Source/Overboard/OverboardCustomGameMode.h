// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OverboardCustomGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API AOverboardCustomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor
	 */
	AOverboardCustomGameMode();

private:
	/**
	* Check all actors in player's camera view
	*/
	void SearchEnemiesInView();

public:

	/**
	 * Called every frame
	 *
	 * @param deltatime between two ticks
	 */
	virtual void Tick(float pDeltaTime) override;
};
