// Fill out your copyright notice in the Description page of Project Settings.


#include "OverboardPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MathHelper.h"
#include "ScreenLogger.h"

// Sets default values
AOverboardPlayer::AOverboardPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_boardContainer = CreateSubObjects<USceneComponent>(RootComponent, "Board container");
	_boardDefaultPosition = CreateSubObjects<USceneComponent>(_boardContainer, "Idle default board position");
	_boardMesh = CreateSubObjects<UStaticMeshComponent>(_boardContainer, "Board mesh");
	_springArm = CreateSubObjects<USpringArmComponent>(RootComponent, "Spring arm");
	_mainCamera = CreateSubObjects<UCameraComponent>(_springArm, "Main camera");
	_boardGroundDetector = CreateSubObjects<USceneComponent>(RootComponent, "Board's ground detector");
}

template<class TSubObjectType>
TSubObjectType* AOverboardPlayer::CreateSubObjects(USceneComponent* pParent, FName pName)
{
	TSubObjectType* lObject = CreateDefaultSubobject<TSubObjectType>(pName);

	if (lObject) 
	{
		lObject->SetupAttachment(pParent);
	}

	return lObject;
}

// Called when the game starts or when spawned
void AOverboardPlayer::BeginPlay()
{
	Super::BeginPlay();
	_currentIdleTime = 0;
	_currentSpeed = 0;
	_previousTurningBoardRoll = 0;
	_currentTurningBoardRollTime = 0;
	_previousBoardRollForStopTurning = 0;
	baseBoardRotation = _boardMesh->GetRelativeRotation();
	_maxSpeed = GetCharacterMovement()->GetMaxSpeed();
	_baseTargetArmLength = _springArm->TargetArmLength;
	_springArmOrientationOffset = _springArm->GetRelativeRotation() - _boardContainer->GetRelativeRotation();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(_defaultMappingContext, 0);
		}
	}
}

// Input

void AOverboardPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (_accelerateInputAction)
	{
		UEnhancedInputComponent* lInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

		if (lInput) 
		{
			// Jumping
			lInput->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			lInput->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

			lInput->BindAction(_accelerateInputAction, ETriggerEvent::Triggered, this, &AOverboardPlayer::ManageAcceleration);
			lInput->BindAction(_accelerateInputAction, ETriggerEvent::None, this, &AOverboardPlayer::StopAccelerate);
			lInput->BindAction(_turnInputAction, ETriggerEvent::Triggered, this, &AOverboardPlayer::Turn);
			lInput->BindAction(_turnInputAction, ETriggerEvent::None, this, &AOverboardPlayer::StopTurning);
		}
	}
	
}

void AOverboardPlayer::ManageAcceleration(const FInputActionInstance& pInstance)
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		float lValue = pInstance.GetValue().Get<float>();

		if (lValue > 0)
		{
			Accelerate(lValue);
		}
		else  if (lValue < 0)
		{
			Brake(lValue);
		}
		else
		{
			Deselerate();
		}
	}
}

void AOverboardPlayer::Accelerate(double pValue)
{
	_isBraking = false;
	_currentSpeed = FMath::Min(_currentSpeed + _acceleration * pValue, _maxSpeed);

	SetBoardPitch(baseBoardRotation.Pitch);

	_currentSpeed = FMath::Max(_currentSpeed - _deceleration, 0);

	LerpCameraArmForAcceleration();

	ApplyNewSpeed();
}

void AOverboardPlayer::LerpCameraArmForAcceleration() 
{
	if (_springArm->TargetArmLength < _accelerationTargetArmLength)
	{
		_springArm->TargetArmLength = FMath::Lerp(_springArm->TargetArmLength, _accelerationTargetArmLength, FMath::Min(_springArmXOffsetAccelerateFactor * _currentSpeed / _maxSpeed, 1.f));
	}
	else if (_springArm->TargetArmLength != _accelerationTargetArmLength)
	{
		_springArm->TargetArmLength = _accelerationTargetArmLength;
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
	_isBraking = false;

	SetBoardPitch(baseBoardRotation.Pitch);

	_currentSpeed = FMath::Max(_currentSpeed - _deceleration, 0);

	LerpCameraArmForDeseleration();

	ApplyNewSpeed();
}

void AOverboardPlayer::Brake(double pValue) 
{
		_isBraking = true;

		_currentSpeed = FMath::Max(_currentSpeed + _brakingStrength * pValue, 0);

		SetBoardPitch(_boardBrakingPitch * FMath::Abs(pValue));

		LerpCameraArmForDeseleration();

		ApplyNewSpeed();
}

void AOverboardPlayer::SetBoardPitch(float pPitch)
{
	FRotator lNewRot = _boardMesh->GetRelativeRotation();
	lNewRot.Pitch = pPitch;
	_boardMesh->SetRelativeRotation(lNewRot);
}

void AOverboardPlayer::LerpCameraArmForDeseleration()
{
	if (_springArm->TargetArmLength > _baseTargetArmLength)
	{
		float lNewTargetArm = FMath::Lerp(_springArm->TargetArmLength, _baseTargetArmLength, _springArmXOffsetAccelerateFactor * FMath::Min(1.f - FMath::Min(_currentSpeed / _maxSpeed, 1.f), 1.f));
		_springArm->TargetArmLength = lNewTargetArm;
	}
	else if (_springArm->TargetArmLength != _baseTargetArmLength)
	{
		_springArm->TargetArmLength = _baseTargetArmLength;
	}
}

void AOverboardPlayer::ApplyNewSpeed() 
{
	// find out which way is forward
	const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, _currentSpeed, true);
}

void AOverboardPlayer::Turn(const FInputActionInstance& pInstance) 
{
	if (GetCharacterMovement()->IsMovingOnGround()) 
	{
		float lValue = pInstance.GetValue().Get<float>() * _turningSpeed;

		if (Controller != nullptr)
		{
			FRotator lRotation = Controller->GetControlRotation();
			lRotation.Yaw += lValue;
			Controller->SetControlRotation(lRotation);

			if (_isBraking)
			{
				SetBoardStopTurningRoll();
			}
			else
			{
				SetBoardTurningRoll(lValue);
			}
		}
	}
	
}

void AOverboardPlayer::SetBoardTurningRoll(float pTurningSpeed)
{
	_currentStopTurningBoardRollTime = 0;
	_previousBoardRollForStopTurning = 0;

	//Cross product to calculate the new rot
	float lNewroll = MathHelper::InvertedCrossProduct(pTurningSpeed * _currentSpeed, _turningSpeed * _maxSpeed,_maxTurningBoardRoll);
	_currentTurningBoardRollTime += GetWorld()->GetDeltaSeconds();
	if (
		SetBoardRoll(
			lNewroll,
			true,
			_previousTurningBoardRoll,
			_currentTurningBoardRollTime,
			_timeToSwipeTurningBoardRoll
		)
	)
	{
		_currentTurningBoardRollTime = 0;
		_previousTurningBoardRoll = lNewroll;
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
	_currentTurningBoardRollTime = 0;
	_previousTurningBoardRoll = 0;

	if (_previousBoardRollForStopTurning == 0) 
	{
		_previousBoardRollForStopTurning = _boardMesh->GetRelativeRotation().Roll;
	}

	_currentStopTurningBoardRollTime += GetWorld()->GetDeltaSeconds();

	if (
		SetBoardRoll(
			baseBoardRotation.Roll, 
			!_isBraking, 
			_previousBoardRollForStopTurning,
			_currentStopTurningBoardRollTime, 
			_timeToSwipeTurningBoardRoll
		)
	)
	{
		_currentStopTurningBoardRollTime = 0;
		_previousBoardRollForStopTurning = 0;
	}
}

bool AOverboardPlayer::SetBoardRoll(float pRoll, bool pLerp, float pPreviousRoll, float pCurrentTime, float pMaxTime)
{
	FRotator lNewRot = _boardMesh->GetRelativeRotation();
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
	_boardMesh->SetRelativeRotation(lNewRot);

	return lResult;
}

void AOverboardPlayer::Tick(float pDeltaTime)
{
	Super::Tick(pDeltaTime);

	if (GetCharacterMovement()->IsMovingOnGround()) 
	{
		FVector lBasePosVector = _boardDefaultPosition->GetRelativeLocation();
		if (_currentSpeed == 0 && !_isBraking)
		{
			lBasePosVector.Z += GetBoardZPositionForIdle(pDeltaTime);
		}
		else
		{
			lBasePosVector.Z = GetBoardZPositionForAcceleration(lBasePosVector.Z);
		}

		_boardMesh->SetRelativeLocation(lBasePosVector);
		SetArmOrientation(pDeltaTime);
	}
}

double AOverboardPlayer::GetBoardZPositionForIdle(float pDeltaTime)
{
	_currentIdleTime += pDeltaTime * _idleSpeed;

	if (_currentIdleTime >= 2 * PI)
	{
		_currentIdleTime = 0;
	}

	return FMath::Sin(_currentIdleTime) * _idleIntensity;
}

double AOverboardPlayer::GetBoardZPositionForAcceleration(double pOldZ) 
{
	float lAccelerationRatio = FMath::Min(_boardZOffsetAccelerateFactor * _currentSpeed / _maxSpeed, 1);
	return FMath::Lerp(pOldZ, pOldZ - _boardZOffsetAccelerate, lAccelerationRatio);
}

void AOverboardPlayer::SetArmOrientation(float pDeltaTime)
{
	FRotator lSpringArmRot = _springArm->GetRelativeRotation();
	FRotator lBoardContainerRot = _boardContainer->GetRelativeRotation();
	double lCurrentPitchOffset = lSpringArmRot.Pitch - lBoardContainerRot.Pitch;
	FRotator lNewRot;
	double lLerpOffset;

	if (FMath::Abs(lSpringArmRot.Pitch - lBoardContainerRot.Pitch - _springArmOrientationOffset.Pitch) >= _springArmOrientationTolerance 
		|| FMath::Abs(lSpringArmRot.Roll - lBoardContainerRot.Roll - _springArmOrientationOffset.Roll) >= _springArmOrientationTolerance)
	{
		_springArmCurrentRotationTime += pDeltaTime;
		lLerpOffset = _springArmRotationSpeed * _springArmCurrentRotationTime;

		if (lLerpOffset >= 1) 
		{
			lLerpOffset = 1;
			_springArmCurrentRotationTime = 0.f;
		}

		lNewRot = FMath::Lerp(lSpringArmRot, _springArmOrientationOffset + lBoardContainerRot, lLerpOffset);
		lNewRot.Yaw = lSpringArmRot.Yaw;
		_springArm->SetRelativeRotation(lNewRot);
	}
	else 
	{
		_springArmCurrentRotationTime = 0.f;
	}
}

