#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "ForceToPositiveOneInputModifier.generated.h"

/**
 * Input modifier to force value to positive
 */
UCLASS()
class OVERBOARD_API UForceToPositiveOneInputModifier : public UInputModifier
{
	GENERATED_BODY()

	/**
	* Modify the input value to be 1
	*
	* @param pPlayerInput The player input instance
	* @param pCurrentValue The current value of the input
	* @param pDeltaTime The delta time since last frame
	*/
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* pPlayerInput, FInputActionValue pCurrentValue, float pDeltaTime) override;
};
