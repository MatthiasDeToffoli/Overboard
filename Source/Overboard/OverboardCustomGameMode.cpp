#include "BaseTargetable.h"
#include "EndScreen.h"
#include "Kismet/GameplayStatics.h"
#include "OverboardCustomGameMode.h"
#include "OverboardHUD.h"
#include "OverboardPlayerController.h"
#include "CountDownScreen.h"
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
	_startScreen = ShowScreen(_startScreenClass);
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
    if (_startScreen)
    {
        _startScreen->RemoveFromParent();
        _startScreen = nullptr;
    }

	_countDownScreen = ShowScreen(_countDownScreenClass);

	if (UCountDownScreen* lCastScreen = Cast< UCountDownScreen>(_countDownScreen))
	{
		lCastScreen->BeginCountdown(_countDownTime);
        UGameplayStatics::SetGamePaused(this, false);
	}
}

void AOverboardCustomGameMode::ShowEndScreen()
{
	_endScreen = Cast<UEndScreen>(ShowScreen(_endScreenClass));

    AOverboardPlayerController* lPlayerController = Cast<AOverboardPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    if (_endScreen && lPlayerController) 
    {
		_endScreen->Init(lPlayerController->GetCurrentScore());
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