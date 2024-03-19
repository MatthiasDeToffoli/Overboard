// Fill out your copyright notice in the Description page of Project Settings.


#include "ForceToNegativeOneInputModifier.h"

FInputActionValue UForceToNegativeOneInputModifier::ModifyRaw_Implementation(const UEnhancedPlayerInput* pPlayerInput, FInputActionValue pCurrentValue, float pDeltaTime)
{
	return -1.f;
}
