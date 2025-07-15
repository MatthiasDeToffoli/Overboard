// Fill out your copyright notice in the Description page of Project Settings.


#include "OverboardHUD.h"
#include "ScreenLogger.h"

void AOverboardHUD::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (_healtBarWidgetClass)
	{
		UScreenLogger::WriteInfo("Creating Health Bar Widget");
		_healtBarWidget = CreateWidget<UHealtBarWidget>(GetWorld(), _healtBarWidgetClass, "Health Bar");

		if (_healtBarWidget)
		{
			UScreenLogger::WriteInfo("Created Health Bar Widget");
			_healtBarWidget->AddToViewport();
			_healtBarWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}


void AOverboardHUD::UpdateHealth(float pHealth, float pMaxHealth)
{
	if (_healtBarWidget)
	{
		_healtBarWidget->UpdateHealth(pHealth, pMaxHealth);
	}
}