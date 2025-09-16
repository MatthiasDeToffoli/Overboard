#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountDownScreen.generated.h"

/**
 * Screen making a countdown to start the game.
 */
UCLASS()
class OVERBOARD_API UCountDownScreen : public UUserWidget
{
	GENERATED_BODY()
	
private:
	/**
	* Current countdown value
	*/
	int _currentCount;

	/**
	* Timer handle for the countdown
	*/
	FTimerHandle _countdownTimerHandle;

	/**
	* Text block showing the countdown value
	*/
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* _countdownText;
	

private:
	/**
	* Update the countdown value and the text block
	*/
	void UpdateCountdown();

public:
	/**
	* Start the countdown
	* 
	* @param pCountDownInit Initial value of the countdown
	*/
	void BeginCountdown(int  pCountDownInit);
};
