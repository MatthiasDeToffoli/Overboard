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
    FVector lCameraLocation;
    FRotator lCameraRotation;
    TArray<AActor*> lEnemiesInView;
    TArray<AActor*> lEnemies;
    FVector lDirectionToEnemy;
    APlayerController* lBaseController = GetWorld()->GetFirstPlayerController();

    if (!lBaseController) return;

    AOverboardPlayerController* lPlayerController = Cast<AOverboardPlayerController>(lBaseController);

    if (!lPlayerController) return;

    lPlayerController->GetPlayerViewPoint(lCameraLocation, lCameraRotation);

    float lCameraFOV = lPlayerController->PlayerCameraManager->GetFOVAngle() + _offsetCameraFOV;
    float lAspectRatio = lPlayerController->PlayerCameraManager->GetCameraCacheView().AspectRatio;

    // Horizontal and vertical half angles in radians
    float lHorHalfFOVRad = FMath::DegreesToRadians(lCameraFOV / 2.0f);
    float lVerHalfFOVRad = FMath::Atan(FMath::Tan(lHorHalfFOVRad) / lAspectRatio);

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseTargetable::StaticClass(), lEnemies);

    for (AActor* lEnemy : lEnemies)
    {
        lDirectionToEnemy = (lEnemy->GetActorLocation() - lCameraLocation).GetSafeNormal();

        //Check horizontal and vertical for more efficient result
        if (FVector::DotProduct(lCameraRotation.Vector(), lDirectionToEnemy) >= FMath::Cos(lHorHalfFOVRad)
            && FMath::Abs(FVector::DotProduct(lDirectionToEnemy, lCameraRotation.Quaternion().GetRightVector())) <= FMath::Tan(lVerHalfFOVRad))
        {
            lEnemiesInView.Add(lEnemy);
        }
    }

    lPlayerController->UpdateEnemiesInView(lEnemiesInView);
}