#include "OverboardPlayer.h"
#include "ActorBuilder.h"
#include "BaseBullet.h"
#include "BaseTargetable.h"
#include <Camera/CameraComponent.h>
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include "HealthComponent.h"
#include <Kismet/GameplayStatics.h>
#include "MathHelper.h"
#include "OverboardCustomGameMode.h"
#include "OverboardHUD.h"
#include "OverboardPlayerController.h"


AOverboardPlayer::AOverboardPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boardContainer_ = UActorBuilder::CreateSubObjects<USceneComponent>(this, RootComponent, "Board container");
	boardDefaultPosition_ = UActorBuilder::CreateSubObjects<USceneComponent>(this, boardContainer_, "Idle default board position");
	boardMesh_ = UActorBuilder::CreateSubObjects<UStaticMeshComponent>(this, boardContainer_, "Board mesh");
	springArm_ = UActorBuilder::CreateSubObjects<USpringArmComponent>(this, RootComponent, "Spring arm");
	mainCamera_ = UActorBuilder::CreateSubObjects<UCameraComponent>(this, springArm_, "Main camera");
	boardGroundDetector_ = UActorBuilder::CreateSubObjects<USceneComponent>(this, RootComponent, "Board's ground detector");
	bulletSpawner_ = UActorBuilder::CreateSubObjects<USceneComponent>(this, RootComponent, "Bullet spawner");
	healthComponent_ = CreateDefaultSubobject<UHealthComponent>("Health");
}

// Called when the game starts or when spawned
void AOverboardPlayer::BeginPlay()
{
	Super::BeginPlay();
	currentIdleTime_ = 0;
	currentSpeed_ = 0;
	previousTurningBoardRoll_ = 0;
	currentTurningBoardRollTime_ = 0;
	previousBoardRollForStopTurning_ = 0;
	springArmAirCurrentRotationTime_ = 0;
	baseBoardRotation_ = boardMesh_->GetRelativeRotation();
	maxSpeed_ = GetCharacterMovement()->GetMaxSpeed();
	baseTargetArmLength_ = springArm_->TargetArmLength;
	springArmOrientationOffset_ = springArm_->GetRelativeRotation() - boardContainer_->GetRelativeRotation();
	isFlying_ = !GetCharacterMovement()->IsMovingOnGround();

	//Add Input Mapping Context
	if (AOverboardPlayerController* lPlayerController = Cast<AOverboardPlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(lPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(defaultMappingContext_, 0);
		}

		//Get HUD
		playerHUD_ = lPlayerController->GetCastHUD();
	}

	

}

// Input

void AOverboardPlayer::SetupPlayerInputComponent(UInputComponent* pPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(pPlayerInputComponent);

	UEnhancedInputComponent* lInput = Cast<UEnhancedInputComponent>(pPlayerInputComponent);

	if (lInput) 
	{
		// Jumping
		lInput->BindAction(jumpInputAction_, ETriggerEvent::Started, this, &ACharacter::Jump);
		lInput->BindAction(jumpInputAction_, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Character movement
		lInput->BindAction(accelerateInputAction_, ETriggerEvent::Triggered, this, &AOverboardPlayer::VerticalMovement);
		lInput->BindAction(accelerateInputAction_, ETriggerEvent::None, this, &AOverboardPlayer::StopAccelerate);
		lInput->BindAction(turnInputAction_, ETriggerEvent::Triggered, this, &AOverboardPlayer::HorizontalMovement);
		lInput->BindAction(turnInputAction_, ETriggerEvent::None, this, &AOverboardPlayer::StopTurning);

		// Camera
		lInput->BindAction(cameraPitchControlInputAction_, ETriggerEvent::Triggered, this, &AOverboardPlayer::CameraControlPitch);
		lInput->BindAction(cameraPitchControlInputAction_, ETriggerEvent::None, this, &AOverboardPlayer::StopCameraControlPitch);
		lInput->BindAction(cameraYawControlInputAction_, ETriggerEvent::Triggered, this, &AOverboardPlayer::CameraControlYaw);
		lInput->BindAction(cameraYawControlInputAction_, ETriggerEvent::None, this, &AOverboardPlayer::StopCameraControlYaw);

		//shoot
		lInput->BindAction(shootInputAction_, ETriggerEvent::Started, this, &AOverboardPlayer::Shoot);
	}
	
	
}

void AOverboardPlayer::CameraControlPitch(const FInputActionInstance& pInstance) 
{
	if (GetCharacterMovement()->IsMovingOnGround()) 
	{
		float lValue = pInstance.GetValue().Get<float>();
		FRotator lRot = springArm_->GetRelativeRotation();
		isControllingCameraPitch_ = true;
		lRot.Pitch -= lValue * cameraOrientationSpeed_;

		if (lRot.Pitch < (maxCameraPitchControlled_ * -1))
		{
			lRot.Pitch = maxCameraPitchControlled_ * -1;
		}
		else if (baseCameraOrientation_.Pitch - lRot.Pitch  < 0)
		{
			lRot.Pitch = baseCameraOrientation_.Pitch;
		}
		
		springArm_->SetRelativeRotation(lRot);
	}
	else 
	{
		isControllingCameraPitch_ = false;
	}
}

void AOverboardPlayer::StopCameraControlPitch(const FInputActionInstance& pInstance)
{
	isControllingCameraPitch_ = false;
}

void AOverboardPlayer::VerticalMovement(const FInputActionInstance& pInstance)
{
	float lValue = pInstance.GetValue().Get<float>();

	if (GetCharacterMovement()->IsMovingOnGround())
	{
		ManageAcceleration(lValue);
	}
	else 
	{
		VerticalAirMovement(lValue);
	}
}

void AOverboardPlayer::VerticalAirMovement(float pValue)
{
	boardContainer_->AddLocalRotation(*(new FRotator(pValue * verticalAirSpeed_ * -1, 0, 0)));
}

void AOverboardPlayer::ManageAcceleration(float pValue)
{
	if (pValue > 0)
	{
		Accelerate(pValue);
	}
	else  if (pValue < 0)
	{
		Brake(pValue);
	}
	else
	{
		Deselerate();
	}
}

void AOverboardPlayer::Accelerate(float pValue)
{
	isBraking_ = false;
	currentSpeed_ = FMath::Min(currentSpeed_ + acceleration_ * pValue, maxSpeed_);

	SetBoardPitch(baseBoardRotation_.Pitch);

	currentSpeed_ = FMath::Max(currentSpeed_ - deceleration_, 0);

	LerpCameraArmForAcceleration();

	ApplyNewSpeed();
}

void AOverboardPlayer::LerpCameraArmForAcceleration() 
{
	if (springArm_->TargetArmLength < accelerationTargetArmLength_)
	{
		springArm_->TargetArmLength = FMath::Lerp(springArm_->TargetArmLength, accelerationTargetArmLength_, FMath::Min(springArmXOffsetAccelerateFactor_ * currentSpeed_ / maxSpeed_, 1.f));
	}
	else if (springArm_->TargetArmLength != accelerationTargetArmLength_)
	{
		springArm_->TargetArmLength = accelerationTargetArmLength_;
	}
}

void AOverboardPlayer::StopAccelerate(const FInputActionInstance& pInstance)
{
	if (GetCharacterMovement()->IsMovingOnGround()) 
	{
		Deselerate();
	}
}

void AOverboardPlayer::Deselerate() 
{
	isBraking_ = false;

	SetBoardPitch(baseBoardRotation_.Pitch);

	currentSpeed_ = FMath::Max(currentSpeed_ - deceleration_, 0);

	LerpCameraArmForDeseleration();

	ApplyNewSpeed();
}

void AOverboardPlayer::Brake(float pValue)
{
		isBraking_ = true;

		currentSpeed_ = FMath::Max(currentSpeed_ + brakingStrength_ * pValue, 0);

		SetBoardPitch(boardBrakingPitch_ * FMath::Abs(pValue));

		LerpCameraArmForDeseleration();

		ApplyNewSpeed();
}

void AOverboardPlayer::SetBoardPitch(float pPitch)
{
	FRotator lNewRot = boardMesh_->GetRelativeRotation();
	lNewRot.Pitch = pPitch;
	boardMesh_->SetRelativeRotation(lNewRot);
}

void AOverboardPlayer::LerpCameraArmForDeseleration()
{
	if (springArm_->TargetArmLength > baseTargetArmLength_)
	{
		float lNewTargetArm = FMath::Lerp(springArm_->TargetArmLength, baseTargetArmLength_, springArmXOffsetAccelerateFactor_ * FMath::Min(1.f - FMath::Min(currentSpeed_ / maxSpeed_, 1.f), 1.f));
		springArm_->TargetArmLength = lNewTargetArm;
	}
	else if (springArm_->TargetArmLength != baseTargetArmLength_)
	{
		springArm_->TargetArmLength = baseTargetArmLength_;
	}
}

void AOverboardPlayer::ApplyNewSpeed() 
{
	// find out which way is forward
	const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, currentSpeed_, true);
}

void AOverboardPlayer::CameraControlYaw(const FInputActionInstance& pInstance)
{
	float lValue = pInstance.GetValue().Get<float>();
	FRotator lRot = springArm_->GetRelativeRotation();
	isControllingCameraYaw_ = true;
	lRot.Yaw += lValue * cameraOrientationSpeed_;

	if (lRot.Yaw < (maxCameraYaw_ * -1))
	{
		lRot.Yaw = maxCameraYaw_ * -1;
	}
	else if (lRot.Yaw > maxCameraYaw_)
	{
		lRot.Yaw = maxCameraYaw_;
	}

	springArm_->SetRelativeRotation(lRot);
}

void AOverboardPlayer::StopCameraControlYaw(const FInputActionInstance& pInstance)
{
	isControllingCameraYaw_ = false;
}

void AOverboardPlayer::HorizontalMovement(const FInputActionInstance& pInstance) 
{
	float lValue = pInstance.GetValue().Get<float>();

	if (GetCharacterMovement()->IsMovingOnGround()) 
	{
		Turn(lValue);
	}
	else 
	{
		HorizontalAirMovement(lValue);
	}
}

void AOverboardPlayer::HorizontalAirMovement(float pValue)
{
	FRotator lBoardContainerRot = boardContainer_->GetRelativeRotation();
	lBoardContainerRot.Yaw += pValue * horizontalAirSpeed_;
	boardContainer_->SetRelativeRotation(lBoardContainerRot);
}

void AOverboardPlayer::Turn(float pValue)
{
	if (Controller != nullptr)
	{
		float lValToAdd = pValue * turningSpeed_;

		FRotator lRotation = Controller->GetControlRotation();
		lRotation.Yaw += lValToAdd;
		Controller->SetControlRotation(lRotation);

		if (isBraking_)
		{
			SetBoardStopTurningRoll();
		}
		else
		{
			SetBoardTurningRoll(lValToAdd);
		}
	}
}

void AOverboardPlayer::SetBoardTurningRoll(float pTurningSpeed)
{
	currentStopTurningBoardRollTime_ = 0;
	previousBoardRollForStopTurning_ = 0;

	//Cross product to calculate the new rot
	float lNewroll = MathHelper::InvertedCrossProduct(pTurningSpeed * currentSpeed_, turningSpeed_ * maxSpeed_,maxTurningBoardRoll_);
	currentTurningBoardRollTime_ += GetWorld()->GetDeltaSeconds();
	if (
		SetBoardRoll(
			lNewroll,
			true,
			previousTurningBoardRoll_,
			currentTurningBoardRollTime_,
			timeToSwipeTurningBoardRoll_
		)
	)
	{
		currentTurningBoardRollTime_ = 0;
		previousTurningBoardRoll_ = lNewroll;
	}
}

void AOverboardPlayer::StopTurning(const FInputActionInstance& pInstance) 
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		SetBoardStopTurningRoll();
	}
}

void AOverboardPlayer::SetBoardStopTurningRoll()
{
	currentTurningBoardRollTime_ = 0;
	previousTurningBoardRoll_ = 0;

	if (previousBoardRollForStopTurning_ == 0) 
	{
		previousBoardRollForStopTurning_ = boardMesh_->GetRelativeRotation().Roll;
	}

	currentStopTurningBoardRollTime_ += GetWorld()->GetDeltaSeconds();

	if (
		SetBoardRoll(
			baseBoardRotation_.Roll, 
			!isBraking_, 
			previousBoardRollForStopTurning_,
			currentStopTurningBoardRollTime_, 
			timeToSwipeTurningBoardRoll_
		)
	)
	{
		currentStopTurningBoardRollTime_ = 0;
		previousBoardRollForStopTurning_ = 0;
	}
}

bool AOverboardPlayer::SetBoardRoll(float pRoll, bool pLerp, float pPreviousRoll, float pCurrentTime, float pMaxTime)
{
	FRotator lNewRot = boardMesh_->GetRelativeRotation();
	bool lResult = true;

	if (pLerp && pCurrentTime < pMaxTime && !FMath::IsNearlyEqual(lNewRot.Roll, pRoll, 0.001))
	{
		lNewRot.Roll = FMath::Lerp(pPreviousRoll, pRoll, pCurrentTime / pMaxTime);
		lResult = false;
	}
	else
	{
		lNewRot.Roll = pRoll;
	}
	boardMesh_->SetRelativeRotation(lNewRot);

	return lResult;
}

void AOverboardPlayer::Tick(float pDeltaTime)
{
	Super::Tick(pDeltaTime);

	if (GetCharacterMovement()->IsMovingOnGround()) 
	{
		FRotator lRot = boardContainer_->GetRelativeRotation();

		if (FMath::Abs(lRot.Yaw) >= 1)
		{
			currentTimeToResetPitchWhenLanding_ += pDeltaTime;
			
			lRot.Yaw = FMath::Lerp(lRot.Yaw, 0,  currentTimeToResetPitchWhenLanding_ / timeToResetPitchWhenLanding_);
			boardContainer_->SetRelativeRotation(lRot);
		}
		else if(lRot.Yaw != 0)
		{
			lRot.Yaw = 0;
		}

		springArmAirCurrentRotationTime_ = 0;
		FVector lBasePosVector = boardDefaultPosition_->GetRelativeLocation();
		if (currentSpeed_ == 0 && !isBraking_)
		{
			lBasePosVector.Z += GetBoardZPositionForIdle(pDeltaTime);
		}
		else
		{
			lBasePosVector.Z = GetBoardZPositionForAcceleration(lBasePosVector.Z);
		}

		boardMesh_->SetRelativeLocation(lBasePosVector);
		SetGroundedArmOrientation(pDeltaTime);
	}
	else 
	{
		isFlying_ = true;
		SetAirArmOrientation(pDeltaTime);
	}

	if (!isControllingCameraYaw_)
	{
		springArmResetYawTime_ += pDeltaTime;
		ResetCameraControlYaw();
	}
	else if (springArmResetYawTime_ != 0)
	{
		springArmResetYawTime_ = 0;
	}

	if (EnemyLocked)
	{
		EnemyLocked->UpdateTargetRotation(GetActorLocation(), pDeltaTime);
	}
}

void AOverboardPlayer::Landing(const FHitResult& pHit)
{
	double lAngle = MathHelper::AngleBetweenVectors(pHit.GetActor()->GetActorUpVector(),boardContainer_->GetUpVector());

	if (lAngle < landingYawTollerance_)
	{
		//Do the win XP here
	}
	else
	{
		boardContainer_->SetRelativeRotation(FRotator::ZeroRotator);
	}

	isFlying_ = false;
}

double AOverboardPlayer::GetBoardZPositionForIdle(float pDeltaTime)
{
	currentIdleTime_ += pDeltaTime * idleSpeed_;

	if (currentIdleTime_ >= 2 * PI)
	{
		currentIdleTime_ = 0;
	}

	return FMath::Sin(currentIdleTime_) * idleIntensity_;
}

double AOverboardPlayer::GetBoardZPositionForAcceleration(double pOldZ) 
{
	float lAccelerationRatio = FMath::Min(boardZOffsetAccelerateFactor_ * currentSpeed_ / maxSpeed_, 1);
	return FMath::Lerp(pOldZ, pOldZ - boardZOffsetAccelerate_, lAccelerationRatio);
}

void AOverboardPlayer::SetGroundedArmOrientation(float pDeltaTime)
{
	springArmCurrentRotationTime_ = SetArmOrientation
	(
		springArmOrientationOffset_, 
		springArmCurrentRotationTime_, 
		pDeltaTime, 
		springArmOrientationTolerance_, 
		springArmRotationSpeed_
	);
}

void AOverboardPlayer::SetAirArmOrientation(float pDeltaTime)
{
	springArmAirCurrentRotationTime_ = SetArmOrientation
	(
		springArmAirOrientationOffset_,
		springArmAirCurrentRotationTime_,
		pDeltaTime,
		springArmAirOrientationTolerance_,
		springArmAirRotationSpeed_
	);

	springArm_->TargetArmLength = FMath::Lerp(springArm_->TargetArmLength, springArmAirLength_, springArmAirCurrentRotationTime_);
}

float AOverboardPlayer::SetArmOrientation(FRotator pWantedRotation, float pTotalTime, float pDeltaTime, float pTolerance, float pSpeed)
{
	if (!isControllingCameraPitch_) 
	{
		FRotator lSpringArmRot = springArm_->GetRelativeRotation();
		FRotator lNewRot;
		double lLerpOffset;

		if (FMath::Abs(lSpringArmRot.Pitch - pWantedRotation.Pitch) >= pTolerance
			|| FMath::Abs(lSpringArmRot.Roll - pWantedRotation.Roll) >= pTolerance)
		{
			pTotalTime += pDeltaTime;
			lLerpOffset = pSpeed * pTotalTime;

			if (lLerpOffset >= 1)
			{
				lLerpOffset = 1;
				pTotalTime = 0.f;
			}

			lNewRot = FMath::Lerp(lSpringArmRot, pWantedRotation, lLerpOffset);
			lNewRot.Yaw = lSpringArmRot.Yaw;
			springArm_->SetRelativeRotation(lNewRot);
		}
		else
		{
			pTotalTime = 0.f;
		}
	}
	

	return pTotalTime;
}

void AOverboardPlayer::ResetCameraControlYaw()
{
	FRotator lSpringArmRot = springArm_->GetRelativeRotation();

	double lNewYaw;
	double lLerpOffset;

	if (FMath::Abs(lSpringArmRot.Pitch - baseCameraOrientation_.Yaw) >= springArmOrientationTolerance_)
	{
		lLerpOffset = springArmRotationSpeed_ * springArmResetYawTime_;

		if (lLerpOffset >= 1)
		{
			lLerpOffset = 1;
			springArmResetYawTime_ = 0.f;
		}

		lNewYaw = FMath::Lerp(lSpringArmRot.Yaw, baseCameraOrientation_.Yaw, lLerpOffset);
		springArm_->SetRelativeRotation(FRotator(lSpringArmRot.Pitch,lNewYaw,lSpringArmRot.Roll));
	}
	else
	{
		springArmResetYawTime_ = 0.f;
	}
}

void AOverboardPlayer::Landed(const FHitResult& Hit)
{
	Landing(Hit);
	currentTimeToResetPitchWhenLanding_ = 0;
	boardGroundDetector_->SetActiveFlag(true);
}

void AOverboardPlayer::EnemiesInViewUpdated(TArray<AActor*> pEnemies)
{
	if ((!&pEnemies || pEnemies.Num() == 0) && EnemyLocked)
	{
		EnemyLocked->SetTargeted(false);
		EnemyLocked = nullptr;
	}
	else if (!EnemyLocked)
	{
		UpdateEnemyLocked(pEnemies);
	}
	else if (!CheckEnemyLockedValididy(pEnemies))
	{
		EnemyLocked->SetTargeted(false);
		UpdateEnemyLocked(pEnemies);
	}
}

bool AOverboardPlayer::CheckEnemyLockedValididy(TArray<AActor*> pEnemies)
{
	return pEnemies.Contains(EnemyLocked);
}


void AOverboardPlayer::UpdateEnemyLocked(TArray<AActor*> pEnemies)
{
	AActor* lClosestEnemy = nullptr;

	// Loop through all actors you want to check
	for (AActor* lEnemy : pEnemies)
	{
		FVector lPlayerLocation = GetActorLocation();
		if (!lClosestEnemy || FVector::Dist(lPlayerLocation, lEnemy->GetActorLocation()) < FVector::Dist(lPlayerLocation, lClosestEnemy->GetActorLocation()))
		{
			lClosestEnemy = lEnemy;
		}
	}

	if (lClosestEnemy)
	{
		EnemyLocked = Cast<ABaseTargetable>(lClosestEnemy);
		EnemyLocked->SetTargeted(true);
	}
}

void AOverboardPlayer::UpdateEnemyLocked(ABaseTargetable* pEnemy)
{
	if (pEnemy)
	{
		if (EnemyLocked)
		{
			EnemyLocked->SetTargeted(false);
		}

		EnemyLocked = pEnemy;
		EnemyLocked->SetTargeted(true);
	}
}

void AOverboardPlayer::Shoot()
{

	if(bulletClass_ && EnemyLocked && bulletSpawner_ && bulletSpeed_ > 0)
	{
		// Get the player’s shooting location (e.g., from a gun socket or camera)
		FVector lSpawnLocation = bulletSpawner_->GetComponentLocation();
		FVector lTargetLocation = EnemyLocked->GetActorLocation();
		FRotator lSpawnRotation = (lTargetLocation - lSpawnLocation).Rotation();

		FActorSpawnParameters lSpawnParams;
		lSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		ABaseBullet* lBullet = GetWorld()->SpawnActor<ABaseBullet>(bulletClass_, lSpawnLocation, lSpawnRotation, lSpawnParams);
		if (lBullet)
		{
			lBullet->Configure(lSpawnLocation, lTargetLocation,bulletSpeed_, bulletDamage_);
		}
	}
}

float AOverboardPlayer::TakeDamage(float pDamageAmount, FDamageEvent const& pDamageEvent, AController* pEventInstigator, AActor* pDamageCauser)
{
	AOverboardCustomGameMode* lGameMode = Cast<AOverboardCustomGameMode>(UGameplayStatics::GetGameMode(this));
	if (healthComponent_->ApplyDamage(pDamageAmount) && lGameMode)
	{
		lGameMode->ShowEndScreen();
	}

	//Update the HUD
	if (playerHUD_)
	{
		playerHUD_->UpdateHealth(healthComponent_->GetHealth(), healthComponent_->GetMaxHealth());
	}

	return pDamageAmount;
}