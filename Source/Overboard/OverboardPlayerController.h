// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OverboardPlayer.h"
#include "OverboardPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class OVERBOARD_API AOverboardPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private :
	/**
	* All enemies in player view
	*/
	TArray<AActor*> enemiesInView;

	/**
	 * Input action to change targeted Ennemy
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _ChangeTargetInputAction;

	bool _CanSelectANewTarget;

private:
	/**
	* Change the tarfet
	* @param pInstance Instance of input values to get the properties needed
	*/
	void ChangeTarget(const FInputActionInstance& pInstance);

	/**
	* Get the next target in view
	*  @param pIndex index of the current target 
	*/
	ABaseTargetable* GetNextTarget(int pIndex);

	/**
	* Get the previous target in view
	* @param pIndex index of the current target 
	*/
	ABaseTargetable* GetPreviousTarget(int pIndex);

	/**
	* To make the player release the button before select another enemy
	*/
	void UnblockTargetChanging();
protected:
	/**
	 * Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;

	/**
	* Call when the input component is setted up
	* Add inputs listeners here
	*/
	virtual void SetupInputComponent() override;

public:
	/**
	* Get the current player on type AOverboardPlayer
	*/
	AOverboardPlayer* GetPlayer();

	/**
	* Update enemies array in player view datas
	* 
	* @param pEnemies all enemies in view
	*/
	void UpdateEnemiesInView(TArray<AActor*> pEnemies);
};
