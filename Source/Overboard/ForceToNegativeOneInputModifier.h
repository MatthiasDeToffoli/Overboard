// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "ForceToNegativeOneInputModifier.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API UForceToNegativeOneInputModifier : public UInputModifier
{
	GENERATED_BODY()

protected:

	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* pPlayerInput, FInputActionValue pCurrentValue, float pDeltaTime) override;
	
};
