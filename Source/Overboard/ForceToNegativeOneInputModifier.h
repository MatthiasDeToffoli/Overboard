#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "ForceToNegativeOneInputModifier.generated.h"

/**
 * Input modifier to force value to negative
 */
UCLASS()
class OVERBOARD_API UForceToNegativeOneInputModifier : public UInputModifier
{
	GENERATED_BODY()

protected:
	/**
	* Modify the input value to be -1
	* 
	* @param pPlayerInput The player input instance
	* @param pCurrentValue The current value of the input
	* @param pDeltaTime The delta time since last frame
	*/
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* pPlayerInput, FInputActionValue pCurrentValue, float pDeltaTime) override;
	
};
