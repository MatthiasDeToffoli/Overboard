// Fill out your copyright notice in the Description page of Project Settings.


#include "OverboardPlayerController.h"
#include <EnhancedInputComponent.h>
#include "ScreenLogger.h"

AOverboardPlayer* AOverboardPlayerController::GetPlayer()
{
	APawn* lPawn = GetPawn();

	if (lPawn)
	{
		return Cast<AOverboardPlayer>(lPawn);
	}

	return nullptr;
}

void AOverboardPlayerController::BeginPlay()
{
	_CanSelectANewTarget = true;
}

void AOverboardPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* lInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (lInput)
	{
		lInput->BindAction(_ChangeTargetInputAction, ETriggerEvent::Triggered, this, &AOverboardPlayerController::ChangeTarget);
		lInput->BindAction(_ChangeTargetInputAction, ETriggerEvent::Completed, this, &AOverboardPlayerController::UnblockTargetChanging);
	}
}

void AOverboardPlayerController::ChangeTarget(const FInputActionInstance& pInstance)
{
	AOverboardPlayer* lPlayer = GetPlayer();
	int lIndex = INDEX_NONE;
	float lInput = pInstance.GetValue().Get<float>();
	ABaseTargetable* lNewTarget;

	//Player is necessary in the all method
	if (!lPlayer || enemiesInView.Num() <= 0)
	{
		return;
	}

	//Get index of the new enemy to lock or set 0
	if (lPlayer->EnemyLocked && enemiesInView.Num() > 1)
	{
		lIndex = enemiesInView.Find(lPlayer->EnemyLocked);
	}

	if (_CanSelectANewTarget)
	{
		if (lIndex == INDEX_NONE)
		{
			lNewTarget = Cast<ABaseTargetable>(enemiesInView[0]);
		}
		else if (lInput < 0)
		{
			lNewTarget = GetPreviousTarget(lIndex);
		}
		else 
		{
			lNewTarget = GetNextTarget(lIndex);
		}

		lPlayer->UpdateEnemyLocked(lNewTarget);
		_CanSelectANewTarget = false;
	}
}

ABaseTargetable* AOverboardPlayerController::GetPreviousTarget(int pIndex)
{
	int lNewIndex = pIndex - 1;

	if (lNewIndex < 0)
	{
		lNewIndex = enemiesInView.Num() - 1;
	}

	return Cast<ABaseTargetable>(enemiesInView[lNewIndex]);
}

ABaseTargetable* AOverboardPlayerController::GetNextTarget(int pIndex)
{
	int lNewIndex = pIndex + 1;

	if (lNewIndex >= enemiesInView.Num())
	{
		lNewIndex = 0;
	}

	return Cast<ABaseTargetable>(enemiesInView[lNewIndex]);
}

void AOverboardPlayerController::UnblockTargetChanging()
{
	_CanSelectANewTarget = true;
}
void AOverboardPlayerController::UpdateEnemiesInView(TArray<AActor*> pEnemies) 
{
	enemiesInView = pEnemies;
	GetPlayer()->EnemiesInViewUpdated(pEnemies);
}