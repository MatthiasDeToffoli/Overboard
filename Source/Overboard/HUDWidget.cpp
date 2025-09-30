#include "HUDWidget.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void UHUDWidget::UpdateHealthBar(float pHealth, float pMaxHealth)
{
	if (healthBar_)
	{
		healthBar_->SetPercent(pHealth / pMaxHealth);
	}
}

void UHUDWidget::UpdateScoreText(int32 pScore)
{
	if (scoreText_)
	{
		scoreText_->SetText(FText::AsNumber(pScore));
	}
}
