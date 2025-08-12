// Fill out your copyright notice in the Description page of Project Settings.


#include "OverboardHUD.h"
#include "ScreenLogger.h"

void AOverboardHUD::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (_hudWidgetClass && !_hudWidget)
	{
		_hudWidget = CreateWidget<UHUDWidget>(GetWorld(), _hudWidgetClass, "Health Bar");

		if (_hudWidget)
		{
			_hudWidget->AddToViewport();


			_hudWidget->SetVisibility(IsHidden() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
	}
}


void AOverboardHUD::UpdateHealth(float pHealth, float pMaxHealth)
{
	if (_hudWidget)
	{
		_hudWidget->UpdateHealthBar(pHealth, pMaxHealth);
	}
}

void AOverboardHUD::UpdateScore(int pScore)
{
	if (_hudWidget)
	{
		_hudWidget->UpdateScoreText(pScore);
	}
}

void AOverboardHUD::HideHUD()
{
	if (_hudWidget)
	{
		_hudWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	Super::SetHidden(true);
}

void AOverboardHUD::ShowHUD()
{
	if (_hudWidget)
	{
		_hudWidget->SetVisibility(ESlateVisibility::Visible);
	}

	Super::ShowHUD();
}

