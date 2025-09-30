#include "OverboardCustomGameMode.h"
#include "BaseTargetable.h"
#include "CountDownScreen.h"
#include "EndScreen.h"
#include <Kismet/GameplayStatics.h>
#include "OverboardHUD.h"
#include "OverboardPlayerController.h"
#include "SpawnerManager.h"

AOverboardCustomGameMode::AOverboardCustomGameMode()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

void AOverboardCustomGameMode::BeginPlay()
{
    Super::BeginPlay();

    ShowStartScreen();
}

void AOverboardCustomGameMode::ShowStartScreen()
{
	startScreen_ = ShowScreen(startScreenClass_);
}

void AOverboardCustomGameMode::StartGame()
{
    if (_countDownScreen)
    {
        _countDownScreen->RemoveFromParent();
        _countDownScreen = nullptr;

        APlayerController* lPlayerController = UGameplayStatics::GetPlayerController(this, 0);
        if (lPlayerController)
        {
            lPlayerController->SetInputMode(FInputModeGameOnly());
            lPlayerController->bShowMouseCursor = false;
			lPlayerController->GetHUD()->ShowHUD();

            if (ASpawnerManager* lSpawnerManager = Cast<ASpawnerManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnerManager::StaticClass())))
			{
				lSpawnerManager->IsSpawningEnabled = true;
			}
        }
    }
}

void AOverboardCustomGameMode::ShowCountDownScreen()
{
    if (startScreen_)
    {
        startScreen_->RemoveFromParent();
        startScreen_ = nullptr;
    }

	_countDownScreen = ShowScreen(countDownScreenClass_);

	if (UCountDownScreen* lCastScreen = Cast< UCountDownScreen>(_countDownScreen))
	{
		lCastScreen->BeginCountdown(countDownTime_);
        UGameplayStatics::SetGamePaused(this, false);
	}
}

void AOverboardCustomGameMode::ShowEndScreen()
{
	endScreen_ = Cast<UEndScreen>(ShowScreen(endScreenClass_));

    AOverboardPlayerController* lPlayerController = Cast<AOverboardPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    if (endScreen_ && lPlayerController) 
    {
		endScreen_->Init(lPlayerController->GetCurrentScore());
    }
}

UUserWidget* AOverboardCustomGameMode::ShowScreen(TSubclassOf<UUserWidget> pScreenClass)
{
	UUserWidget* lScreen = nullptr;

    if (pScreenClass)
    {
        AOverboardPlayerController* lPlayerController = Cast<AOverboardPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
        
        if (lPlayerController)
        {
			lPlayerController->GetCastHUD()->HideHUD();

            lScreen = CreateWidget<UUserWidget>(lPlayerController, pScreenClass);
            if (lScreen)
            {
                lScreen->AddToViewport();

                lPlayerController->SetInputMode(FInputModeUIOnly());
                lPlayerController->bShowMouseCursor = true;

                UGameplayStatics::SetGamePaused(this, true);
            }
        }
    }

	return lScreen;
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

    float lCameraFOV = lPlayerController->PlayerCameraManager->GetFOVAngle() + offsetCameraFOV_;
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