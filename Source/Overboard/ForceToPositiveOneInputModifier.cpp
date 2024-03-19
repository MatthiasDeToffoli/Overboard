// Fill out your copyright notice in the Description page of Project Settings.


#include "ForceToPositiveOneInputModifier.h"

FInputActionValue UForceToPositiveOneInputModifier::ModifyRaw_Implementation(const UEnhancedPlayerInput* pPlayerInput, FInputActionValue pCurrentValue, float pDeltaTime)
{
	return 1.f;
}
