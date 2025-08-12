// Fill out your copyright notice in the Description page of Project Settings.


#include "CountDownScreen.h"
#include "Components/TextBlock.h"
#include <Kismet/GameplayStatics.h>
#include "OverboardCustomGameMode.h"

void UCountDownScreen::BeginCountdown(int  pCountDownInit)
{
    _currentCount = pCountDownInit;

    UpdateCountdown();

    GetWorld()->GetTimerManager().SetTimer(
        _countdownTimerHandle,
        this,
        &UCountDownScreen::UpdateCountdown,
        1.0f,
        true
    );
}

void UCountDownScreen::UpdateCountdown()
{
    if (_countdownText)
    {
        if (_currentCount > 0)
        {
            _countdownText->SetText(FText::AsNumber(_currentCount));
        }
        else if(_currentCount > -1)
        {
            _countdownText->SetText(FText::FromString(TEXT("Start!")));
        }
        else 
        {
            // Stop timer
            GetWorld()->GetTimerManager().ClearTimer(_countdownTimerHandle);

            // Call GameMode StartGame()
            if (AOverboardCustomGameMode* lGameMode = Cast<AOverboardCustomGameMode>(UGameplayStatics::GetGameMode(this)))
            {
                lGameMode->StartGame();
            }

            // Optionally: remove widget after short delay
            RemoveFromParent();
        }

        --_currentCount;
    }
}