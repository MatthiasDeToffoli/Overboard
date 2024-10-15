// Fill out your copyright notice in the Description page of Project Settings.


#include "OverboardCustomGameMode.h"
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
    FVector lPlayerLocation = lPlayerController->GetPawn()->GetActorLocation();
    FRotator lCameraRotation;
    lPlayerController->GetPlayerViewPoint(lCameraLocation, lCameraRotation);

    FVector CameraForward = lCameraRotation.Vector();
    float CameraFOV = lPlayerController->PlayerCameraManager->GetFOVAngle();

    TArray<AActor*> lEnemies;
    AActor* lClosestEnemy = nullptr;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Enemy", lEnemies);

    // Loop through all actors you want to check
    for (AActor* lEnemy : lEnemies)
    {
        FVector lActorLocation = lEnemy->GetActorLocation();

        FVector lDirectionToActor = (lActorLocation - lCameraLocation).GetSafeNormal();
        float lDotProduct = FVector::DotProduct(CameraForward, lDirectionToActor);

        // Convert FOV from degrees to radians for comparison
        float lHalfFOVRadians = FMath::DegreesToRadians(CameraFOV / 2.0f);
        float lCosHalfFOV = FMath::Cos(lHalfFOVRadians);

        // If the dot product is greater than cos of half FOV, actor is in view
        if (lDotProduct >= lCosHalfFOV)
        {
            UScreenLogger::WriteInfo(FString::Printf(TEXT("%s is within view"), *lEnemy->GetName()));
        }

        // If the dot product is greater than cos of half FOV, actor is in view
        if (lDotProduct >= lCosHalfFOV && 
            (!lClosestEnemy|| FVector::Dist(lPlayerLocation, lActorLocation) < FVector::Dist(lPlayerLocation, lClosestEnemy->GetActorLocation())))
        {
            //Keep only the closest enemy
            lClosestEnemy = lEnemy;
        }
    }

    if (lClosestEnemy)
    {
        UScreenLogger::WriteInfo(FString::Printf(TEXT("Enemy found %s"), *lClosestEnemy->GetName()));
    }
}