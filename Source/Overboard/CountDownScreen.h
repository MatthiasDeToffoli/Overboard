// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountDownScreen.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API UCountDownScreen : public UUserWidget
{
	GENERATED_BODY()
	
private:
	int _currentCount;

	FTimerHandle _countdownTimerHandle;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* _countdownText;
	

private:

	void UpdateCountdown();

public:
	// Start countdown
	void BeginCountdown(int  pCountDownInit);
};
