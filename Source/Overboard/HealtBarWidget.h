// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ProgressBar.h>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealtBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API UHealtBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* _healthBar;

public:
	/**
	 * Update the health bar
	 * @param pHealth Current health of the player
	 * @param pMaxHealth Maximum health of the player
	 */
	void UpdateHealth(float pHealth, float pMaxHealth);
};
