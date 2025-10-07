#pragma once

#include "CoreMinimal.h"
#include <GameFramework/GameModeBase.h>
#include "OverboardCustomGameMode.generated.h"

class UEndScreen;

/**
 * Custom game mode to manage the game flow
 */
UCLASS()
class OVERBOARD_API AOverboardCustomGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	/**
	* offset for camera FOV to detect enemies in a largest zone
	*/
	UPROPERTY(EditAnywhere, Category = "Enemy targeting")
	float offsetCameraFOV_ = 20;

	/**
	 * Class of the start screen to show at the beginning of the game
	 */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> startScreenClass_;

	/**
	 * Class of the countdown screen to show before the game starts
	 */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> countDownScreenClass_;

	/**
	 * Class of the end screen to show at the end of the game
	 */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEndScreen> endScreenClass_;

	/**
	 * Start screen widget
	 *
	 * This screen will be shown at the beginning of the game
	 */
	UPROPERTY()
	UUserWidget* startScreen_;
	
	/**
	 * Start screen widget
	 *
	 * This screen will be shown at the beginning of the game
	 */
	UPROPERTY()
	UUserWidget* countDownScreen_;

	UPROPERTY(EditAnywhere, Category = "UI")
	int countDownTime_ = 3;

	/**
	* End screen widget
	* 
	* This screen will be shown at the end of the game
	*/
	UPROPERTY()
	UEndScreen* endScreen_;

public:
	/**
	 * Default constructor
	 */
	AOverboardCustomGameMode();

private:

	/**
	* Show the start screen
	*/
	void ShowStartScreen();

	/**
	* Show a screen
	* 
	* @param pScreenClass class of the screen to show
	*/
	UUserWidget* ShowScreen(TSubclassOf<UUserWidget> pScreenClass);

	/**
	* Check all actors in player's camera view
	*/
	void SearchEnemiesInView();

public:

	/**
	 * Called every frame
	 *
	 * @param pDeltatime between two ticks
	 */
	virtual void Tick(float pDeltaTime) override;

	/**
	* Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;

	/**
	* Show and Init the countdown screen
	*/
	void ShowCountDownScreen();

	/**
	 * Start the game
	 */
	void StartGame();

	/**
	 * Show the end screen
	 */
	void ShowEndScreen();
};
