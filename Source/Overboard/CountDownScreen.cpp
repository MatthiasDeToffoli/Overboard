#include "CountDownScreen.h"
#include <Components/TextBlock.h>
#include <Kismet/GameplayStatics.h>
#include "OverboardCustomGameMode.h"

void UCountDownScreen::BeginCountdown(int  pCountDownInit)
{
    currentCount_ = pCountDownInit;

    UpdateCountdown();

    GetWorld()->GetTimerManager().SetTimer(
        countdownTimerHandle_,
        this,
        &UCountDownScreen::UpdateCountdown,
        1.0f,
        true
    );
}

void UCountDownScreen::UpdateCountdown()
{
    if (countdownText_)
    {
        if (currentCount_ > 0)
        {
            countdownText_->SetText(FText::AsNumber(currentCount_));
        }
        else if(currentCount_ > -1)
        {
            countdownText_->SetText(FText::FromString(TEXT("Start!")));
        }
        else 
        {
            // Stop timer
            GetWorld()->GetTimerManager().ClearTimer(countdownTimerHandle_);

            // Call GameMode StartGame()
            if (AOverboardCustomGameMode* lGameMode = Cast<AOverboardCustomGameMode>(UGameplayStatics::GetGameMode(this)))
            {
                lGameMode->StartGame();
            }

            // Optionally: remove widget after short delay
            RemoveFromParent();
        }

        --currentCount_;
    }
}