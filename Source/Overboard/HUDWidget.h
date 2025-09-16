#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * Widget showing the player HUD (Health bar, score, etc.)
 */
UCLASS()
class OVERBOARD_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	/**
	 * Health bar showing the player's health
	 */
	UPROPERTY(meta = (BindWidget))
	UProgressBar* _healthBar;

	/**
	 * Text block showing the player's score
	 */
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
