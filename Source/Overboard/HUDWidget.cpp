#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include "HUDWidget.h"

void UHUDWidget::UpdateHealthBar(float pHealth, float pMaxHealth)
{
	if (_healthBar)
	{
		_healthBar->SetPercent(pHealth / pMaxHealth);
	}
}

void UHUDWidget::UpdateScoreText(int32 pScore)
{
	if (_scoreText)
	{
		_scoreText->SetText(FText::AsNumber(pScore));
	}
}
