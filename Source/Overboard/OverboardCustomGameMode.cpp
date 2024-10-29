// Fill out your copyright notice in the Description page of Project Settings.


#include "OverboardCustomGameMode.h"
#include "BaseTargetable.h"
#include "Kismet/GameplayStatics.h"
#include "OverboardPlayerController.h"
#include "ScreenLogger.h"

AOverboardCustomGameMode::AOverboardCustomGameMode()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

void AOverboardCustomGameMode::Tick(float pDeltaTime) 
{
    Super::Tick(pDeltaTime);
    SearchEnemiesInView();
}

void AOverboardCustomGameMode::SearchEnemiesInView()
{
    APlayerController* lBaseController = GetWorld()->GetFirstPlayerController();

    if (!lBaseController) return;

    AOverboardPlayerController* lPlayerController = Cast<AOverboardPlayerController>(lBaseController);

    if (!lPlayerController) return;

    FVector lCameraLocation;
    FRotator lCameraRotation;
    lPlayerController->GetPlayerViewPoint(lCameraLocation, lCameraRotation);

    FVector CameraForward = lCameraRotation.Vector();
    float CameraFOV = lPlayerController->PlayerCameraManager->GetFOVAngle();

    TArray<AActor*> lEnemiesInView;
    TArray<AActor*> lEnemies;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseTargetable::StaticClass(), lEnemies);

    // Loop through all actors you want to check
    for (AActor* lEnemy : lEnemies)
    {
        float lDotProduct = FVector::DotProduct(CameraForward, (lEnemy->GetActorLocation() - lCameraLocation).GetSafeNormal());

        // Convert FOV from degrees to radians for comparison
        float lCosHalfFOV = FMath::Cos(FMath::DegreesToRadians(CameraFOV / 2.0f));

        // If the dot product is greater than cos of half FOV, actor is in view
        if (lDotProduct >= lCosHalfFOV)
        {
            lEnemiesInView.Add(lEnemy);
        }
    }

    lPlayerController->UpdateEnemiesInView(lEnemies);
}