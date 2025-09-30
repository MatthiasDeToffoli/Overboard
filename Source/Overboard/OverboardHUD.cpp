#include "OverboardHUD.h"
#include "HUDWidget.h"

void AOverboardHUD::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (hudWidgetClass_ && !hudWidget_)
	{
		hudWidget_ = CreateWidget<UHUDWidget>(GetWorld(), hudWidgetClass_, "Health Bar");

		if (hudWidget_)
		{
			hudWidget_->AddToViewport();


			hudWidget_->SetVisibility(IsHidden() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
	}
}

void AOverboardHUD::UpdateHealth(float pHealth, float pMaxHealth)
{
	if (hudWidget_)
	{
		hudWidget_->UpdateHealthBar(pHealth, pMaxHealth);
	}
}

void AOverboardHUD::UpdateScore(int pScore)
{
	if (hudWidget_)
	{
		hudWidget_->UpdateScoreText(pScore);
	}
}

void AOverboardHUD::HideHUD()
{
	if (hudWidget_)
	{
		hudWidget_->SetVisibility(ESlateVisibility::Hidden);
	}

	Super::SetHidden(true);
}

void AOverboardHUD::ShowHUD()
{
	if (hudWidget_)
	{
		hudWidget_->SetVisibility(ESlateVisibility::Visible);
	}

	Super::ShowHUD();
}

