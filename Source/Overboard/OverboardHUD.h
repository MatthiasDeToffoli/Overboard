// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDWidget.h"
#include "OverboardHUD.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API AOverboardHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	/**
	* Reference to the HUD Widget Blueprint class
	*/
	UPROPERTY(EditAnywhere)
	TSubclassOf<UHUDWidget> _hudWidgetClass;

	/**
	* Actual widget instance
	*/
	UHUDWidget* _hudWidget;

protected:
	/**
	 * Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;

public:

	/**
	 * Update the health bar
	 * @param pHealth Current health of the player
	 * @param pMaxHealth Maximum health of the player
	 */
	void UpdateHealth(float pHealth, float pMaxHealth);

	/**
	* Update the score text
	* 
	* @param pScore Current score of the player
	*/
	void UpdateScore(int pScore);
};
