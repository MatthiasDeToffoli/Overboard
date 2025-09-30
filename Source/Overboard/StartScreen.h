// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartScreen.generated.h"

/**
 * Screen representing the start of the  game
 */
UCLASS()
class OVERBOARD_API UStartScreen : public UUserWidget
{
	GENERATED_BODY()

protected:
	/**
	* Native handler for mouse button down events.
	*/
	virtual FReply NativeOnMouseButtonDown(const FGeometry& pInGeometry, const FPointerEvent& pInMouseEvent) override;
	
};
