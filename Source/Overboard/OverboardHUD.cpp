// Fill out your copyright notice in the Description page of Project Settings.


#include "OverboardHUD.h"
#include "ScreenLogger.h"

void AOverboardHUD::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (_hudWidgetClass)
	{
		UScreenLogger::WriteInfo("Creating Health Bar Widget");
		_hudWidget = CreateWidget<UHUDWidget>(GetWorld(), _hudWidgetClass, "Health Bar");

		if (_hudWidget)
		{
			UScreenLogger::WriteInfo("Created Health Bar Widget");
			_hudWidget->AddToViewport();
			_hudWidget->SetVisibility(ESlateVisibility::Visible);
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