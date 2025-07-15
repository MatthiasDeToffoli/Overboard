// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HealtBarWidget.h"
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
	TSubclassOf<UHealtBarWidget> _healtBarWidgetClass;

	/**
	* Actual widget instance
	*/
	UHealtBarWidget* _healtBarWidget;

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
};
