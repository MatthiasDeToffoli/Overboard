#include "ForceToPositiveOneInputModifier.h"

FInputActionValue UForceToPositiveOneInputModifier::ModifyRaw_Implementation(const UEnhancedPlayerInput* pPlayerInput, FInputActionValue pCurrentValue, float pDeltaTime)
{
	return 1.f;
}
