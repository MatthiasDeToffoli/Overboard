// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* _healthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* _scoreText;

public:
	/**
	 * Update the health bar
	 * @param pHealth Current health of the player
	 * @param pMaxHealth Maximum health of the player
	 */
	void UpdateHealthBar(float pHealth, float pMaxHealth);

	/**
	 * Update the score text
	 * @param pScore Current score of the player
	 */
	void UpdateScoreText(int32 pScore);
	
};
