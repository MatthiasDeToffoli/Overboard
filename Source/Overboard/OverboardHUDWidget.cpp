// Fill out your copyright notice in the Description page of Project Settings.


#include "OverboardHUDWidget.h"
#include "ScreenLogger.h"

void UOverboardHUDWidget::UpdateHealth(float pHealth, float pMaxHealth)
{
	UScreenLogger::WriteInfo("Update health");
	
	if (_healthBar)
	{
		UScreenLogger::WriteInfo(FString::Printf(TEXT("Health: %f/%f"), pHealth, pMaxHealth));
		_healthBar->SetPercent(pHealth / pMaxHealth);
	}
}