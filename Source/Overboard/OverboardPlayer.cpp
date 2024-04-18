// Fill out your copyright notice in the Description page of Project Settings.


#include "OverboardPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	baseBoardRotationPitch = _boardMesh->GetComponentRotation().Yaw;
	_maxSpeed = GetCharacterMovement()->GetMaxSpeed();
	_baseTargetArmLength = _springArm->TargetArmLength;

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
			lInput->BindAction(_accelerateInputAction, ETriggerEvent::Triggered, this, &AOverboardPlayer::ManageAcceleration);
			lInput->BindAction(_accelerateInputAction, ETriggerEvent::None, this, &AOverboardPlayer::StopAccelerate);
			lInput->BindAction(_turnInputAction, ETriggerEvent::Triggered, this, &AOverboardPlayer::Turn);
		}
	}
	
}

void AOverboardPlayer::ManageAcceleration(const FInputActionInstance& pInstance)
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

void AOverboardPlayer::Accelerate(double pValue)
{
	_currentSpeed = FMath::Min(_currentSpeed + _acceleration * pValue, _maxSpeed);

	FRotator lNewRot = _boardMesh->GetComponentRotation();
	lNewRot.Pitch = baseBoardRotationPitch;

	_boardMesh->SetWorldRotation(lNewRot);

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
	Deselerate();
}

void AOverboardPlayer::Deselerate() 
{
	FRotator lNewRot = _boardMesh->GetComponentRotation();
	lNewRot.Pitch = baseBoardRotationPitch;

	_boardMesh->SetWorldRotation(lNewRot);

	_currentSpeed = FMath::Max(_currentSpeed - _deceleration, 0);

	LerpCameraArmForDeseleration();

	ApplyNewSpeed();
}

void AOverboardPlayer::Brake(double pValue) 
{
	_currentSpeed = FMath::Max(_currentSpeed + _brakingStrength * pValue, 0);

	FRotator lNewRot = _boardMesh->GetComponentRotation();
	lNewRot.Pitch = _boardBrakingPitch * FMath::Abs(pValue);

	_boardMesh->SetWorldRotation(lNewRot);

	LerpCameraArmForDeseleration();

	ApplyNewSpeed();
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
	float lValue = pInstance.GetValue().Get<float>();

	if (Controller != nullptr) 
	{
		FRotator lRotation = Controller->GetControlRotation();
		lRotation.Yaw += lValue * _turningSpeed;
		Controller->SetControlRotation(lRotation);
	}
}

void AOverboardPlayer::Tick(float pDeltaTime)
{
	Super::Tick(pDeltaTime);

	FVector lBasePosVector = _boardDefaultPosition->GetComponentLocation();
	if (_currentSpeed == 0 && _boardMesh->GetComponentRotation().Pitch == baseBoardRotationPitch)
	{
		lBasePosVector.Z += GetBoardZPositionForIdle(pDeltaTime);
	}
	else 
	{
		lBasePosVector.Z = GetBoardZPositionForAcceleration(lBasePosVector.Z);
	}

	_boardMesh->SetWorldLocation(lBasePosVector);
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

