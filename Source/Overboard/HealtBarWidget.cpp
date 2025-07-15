// Fill out your copyright notice in the Description page of Project Settings.


#include "HealtBarWidget.h"

void UHealtBarWidget::UpdateHealth(float pHealth, float pMaxHealth)
{
	if (_healthBar)
	{
		_healthBar->SetPercent(pHealth / pMaxHealth);
	}
}