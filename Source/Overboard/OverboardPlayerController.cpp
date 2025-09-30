#include "OverboardPlayerController.h"
#include "BaseTargetable.h"
#include <EnhancedInputComponent.h>
#include "OverboardHUD.h"
#include "OverboardPlayer.h"

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
	canSelectANewTarget_ = true;
	SetScoreValue(0);
}

void AOverboardPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* lInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (lInput)
	{
		lInput->BindAction(changeTargetInputAction_, ETriggerEvent::Triggered, this, &AOverboardPlayerController::ChangeTarget);
		lInput->BindAction(changeTargetInputAction_, ETriggerEvent::Completed, this, &AOverboardPlayerController::UnblockTargetChanging);
	}
}

void AOverboardPlayerController::ChangeTarget(const FInputActionInstance& pInstance)
{
	AOverboardPlayer* lPlayer = GetPlayer();
	int lIndex = INDEX_NONE;
	float lInput = pInstance.GetValue().Get<float>();
	ABaseTargetable* lNewTarget;

	//Player is necessary in the all method
	if (!lPlayer || enemiesInView_.Num() <= 0)
	{
		return;
	}

	//Get index of the new enemy to lock or set 0
	if (lPlayer->EnemyLocked && enemiesInView_.Num() > 1)
	{
		lIndex = enemiesInView_.Find(lPlayer->EnemyLocked);
	}

	if (canSelectANewTarget_)
	{
		if (lIndex == INDEX_NONE)
		{
			lNewTarget = Cast<ABaseTargetable>(enemiesInView_[0]);
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
		canSelectANewTarget_ = false;
	}
}

ABaseTargetable* AOverboardPlayerController::GetPreviousTarget(int pIndex)
{
	int lNewIndex = pIndex - 1;

	if (lNewIndex < 0)
	{
		lNewIndex = enemiesInView_.Num() - 1;
	}

	return Cast<ABaseTargetable>(enemiesInView_[lNewIndex]);
}

ABaseTargetable* AOverboardPlayerController::GetNextTarget(int pIndex)
{
	int lNewIndex = pIndex + 1;

	if (lNewIndex >= enemiesInView_.Num())
	{
		lNewIndex = 0;
	}

	return Cast<ABaseTargetable>(enemiesInView_[lNewIndex]);
}

void AOverboardPlayerController::UnblockTargetChanging()
{
	canSelectANewTarget_ = true;
}
void AOverboardPlayerController::UpdateEnemiesInView(TArray<AActor*> pEnemies) 
{
	enemiesInView_ = pEnemies;
	GetPlayer()->EnemiesInViewUpdated(pEnemies);
}

AOverboardHUD* AOverboardPlayerController::GetCastHUD()
{
	if (AOverboardHUD* lHUD = Cast<AOverboardHUD>(GetHUD()))
	{
		return lHUD;
	}
	return nullptr;
}

void AOverboardPlayerController::SetScoreValue(int pVal)
{
	currentScore_ = pVal;

	if (AOverboardHUD* lHUD = GetCastHUD())
	{
		lHUD->UpdateScore(currentScore_);
	}
}

void AOverboardPlayerController::UpdateScore(int pScore)
{
	SetScoreValue(currentScore_ + pScore);
}

int AOverboardPlayerController::GetCurrentScore() const
{
	return currentScore_;
}