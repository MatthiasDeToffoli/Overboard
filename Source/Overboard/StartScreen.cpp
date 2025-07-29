// Fill out your copyright notice in the Description page of Project Settings.


#include "StartScreen.h"
#include "OverboardCustomGameMode.h"
#include <Kismet/GameplayStatics.h>

FReply UStartScreen::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // Get reference to your custom GameMode
    if (APlayerController* lPlayerComp = GetOwningPlayer())
    {
        if (AOverboardCustomGameMode* lGameMode = Cast<AOverboardCustomGameMode>(UGameplayStatics::GetGameMode(this)))
        {
            lGameMode->ShowCountDownScreen();
        }
    }

    return FReply::Handled();  // Consume the click
}