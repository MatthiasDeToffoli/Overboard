// Fill out your copyright notice in the Description page of Project Settings.


#include "StartScreen.h"
#include <Kismet/GameplayStatics.h>
#include "OverboardCustomGameMode.h"

FReply UStartScreen::NativeOnMouseButtonDown(const FGeometry& pInGeometry, const FPointerEvent& pInMouseEvent)
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