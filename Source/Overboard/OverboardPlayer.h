// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OverboardPlayer.generated.h"

struct FInputActionInstance;
struct FInputActionValue;
class UCameraComponent;
class UInputAction;
class UInputComponent;
class UInputMappingContext;
class USpringArmComponent;

UCLASS()
class OVERBOARD_API AOverboardPlayer : public ACharacter
{
	GENERATED_BODY()

private:

	// Camera ----------------------------------------------------------------------------------------------------

	/**
	 *Spring arm for main camera 
	 */
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* _springArm;

	/**
	 * Main camera
	 */
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* _mainCamera;

	// Board ----------------------------------------------------------------------------------------------------
	/**
	 * Default position of the board, used for doing some calculation on the board movement
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board", meta = (AllowPrivateAccess = "true"))
	USceneComponent* _boardContainer;

	/**
	 * Mesh of the board we can move with some actions 
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _boardMesh;

	/**
	 * Default position of the board, used for doing some calculation on the board movement
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board", meta = (AllowPrivateAccess = "true"))
	USceneComponent* _boardDefaultPosition;


	// Input ----------------------------------------------------------------------------------------------------

	/** 
	 * default MappingContext
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* _defaultMappingContext;

	/**
	 * Input action to accelerate
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* _accelerateInputAction;

	/**
	 * Input action to turn
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* _turnInputAction;


	// Idle -----------------------------------------------------------------------------------------------------

	/**
	 * Speed if the board idle's movement
	 */
	UPROPERTY(EditAnywhere, Category = "Idle")
	float _idleSpeed = 1.f;

	/**
	 * Intensity of the board idle's movement
	 */
	UPROPERTY(EditAnywhere, Category = "Idle")
	float _idleIntensity = 1.f;

	/**
	 * current time used for idle movement calculation
	 */
	float _currentIdleTime = 0.f;


	//Movement ---------------------------------------------------------------------------------------------------

	/**
	 * Accelaration value
	 */
	UPROPERTY(EditAnywhere, Category = "Movement/Acceleration")
	float _acceleration = 1.f;

	/**
	 * Used to calculate the Z position of the board when we're on the max speed value
	 */
	UPROPERTY(EditAnywhere, Category = "Movement/Acceleration")
	float _boardZOffsetAccelerate = 1.f;

	/**
	 * Factor to accelerate the time to put the board on the new z position
	 */
	UPROPERTY(EditAnywhere, Category = "Movement/Acceleration")
	float _boardZOffsetAccelerateFactor = 2.f;

	/**
	 * Length of the arm when we're on the maximal speed
	 */
	UPROPERTY(EditAnywhere, Category = "Movement/Acceleration")
	float _accelerationTargetArmLength = 500.f;

	/**
	 * Factor to accelerate the time to put the srping arm to it's new length
	 */
	UPROPERTY(EditAnywhere, Category = "Movement/Acceleration")
	float _springArmXOffsetAccelerateFactor = 2.f;

	/**
	 * Max player speed, got in the ACharacter property
	 */
	float _maxSpeed = 100.f;

	/**
	 * Deceleration value
	 */
	UPROPERTY(EditAnywhere, Category = "Movement/Deceleration")
	float _deceleration = 1.f;

	/**
	 * braking strength value
	 */
	UPROPERTY(EditAnywhere, Category = "Movement/Deceleration")
	float _brakingStrength = 1.f;

	/**
	 * Braking board pitch value.
	 */
	UPROPERTY(EditAnywhere, Category = "Movement/Deceleration")
	float _boardBrakingPitch = 35.f;

	/**
	 * Base target arm length when the character doesn't move
	 */
	float _baseTargetArmLength = 300.f;

	/**
	 * base pitch board value for reset it where the player doesn't try to brake
	 */
	float baseBoardRotationPitch = 0.f;

	/**
	 * Current player speed
	 */
	float _currentSpeed = 0.f;

	/**
	 * turning speed
	 */
	UPROPERTY(EditAnywhere, Category = "Movement Turning")
	float _turningSpeed = 2.f;

public:
	/**
	 * Default constructor
	 */
	AOverboardPlayer();

private:

	/** 
	 * Called to accelerate, deselerate or brake
	 *
	 * @param pInstance input instance used
	 */
	void ManageAcceleration(const FInputActionInstance& pInstance);

	/**
	 * Calculate and apply the acceleration
	 * 
	 * @param pValue Value got by the player input axis
	 */
	void Accelerate(double pValue);

	/**
	 * Move the camera back to see the acceleration effect
	 */
	void LerpCameraArmForAcceleration();

	/**
	 * Called when the player stop to accelerate and doesn't brake with a keyboard
	 *
	 * @param pInstance input instance used
	 */
	void StopAccelerate(const FInputActionInstance& pInstance);

	/**
	 * Calculate and apply the Deseleration (when player input axis is 0)
	 */
	void Deselerate();

	/**
	 * Calculate and apply the brake (when player input axis is under 0)
	 * 
	 * @param pInstance input instance used
	 */
	void Brake(double pValue);

	/**
	 * Move the camera to it base position to see the deseleration effect
	 */
	void LerpCameraArmForDeseleration();

	/**
	 * Apply the new speed calculate with acceleration
	 */
	void ApplyNewSpeed();

	/**
	 * Called when the player try to turn
	 *
	 * @param pInstance input instance used
	 */
	void Turn(const FInputActionInstance& pInstance);

	/**
	 * Calculate and give the board's Z local position to do the idle movement
	 * 
	 * @param pDeltaTime deltatime between two ticks
	 */
	double GetBoardZPositionForIdle(float pDeltaTime);

	/**
	 * Calculate and give the board's Z local position to do the acceleration movement
	 *
	 * @param pOldZ Z local position of the board before the calculation
	 */
	double GetBoardZPositionForAcceleration(double pOldZ);

protected:

	/**
	 * Create a subobject added to the character
	 */
	template<class TSubObjectType>
	TSubObjectType* CreateSubObjects(USceneComponent* pParent, FName pName);

	/**
	 * Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;

	/**
	 * Setup the player input component
	 * 
	 * @param the input component to set
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* pPlayerInputComponent) override;

public:	

	/**
	 * Called every frame
	 * 
	 * @param deltatime between two ticks
	 */
	virtual void Tick(float pDeltaTime) override;



	
};
