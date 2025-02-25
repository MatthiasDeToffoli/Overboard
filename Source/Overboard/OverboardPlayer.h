	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseTargetable.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "OverboardPlayer.generated.h"

class ABaseBullet;
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
	 *Offset between spring arm orientation and board container orientation
	 */
	FRotator _springArmOrientationOffset;

	/**
	* speed of the spring arm rotation lerp per second
	*/
	UPROPERTY(EditAnywhere, Category = "Camera")
	float _springArmRotationSpeed = 0.2f;

	/**
	* Spring arm orientation tolerance for pitch and roll
	*/
	UPROPERTY(EditAnywhere, Category = "Camera")
	float _springArmOrientationTolerance = 0.001f;

	/**
	* Time since the spring arm start to re ajuste it's rotation
	*/
	float _springArmCurrentRotationTime;

	/**
	* Spring arm length when the character is not on ground
	*/
	UPROPERTY(EditAnywhere, Category = "Camera")
	float SpringArmAirLength = 400;

	/**
	 *Offset between spring arm orientation when the character is not on ground
	 */
	UPROPERTY(EditAnywhere, Category = "Camera")
	FRotator _springArmAirOrientationOffset;

	/**
	* speed of the spring arm rotation lerp per secondwhen the character is not on ground
	*/
	UPROPERTY(EditAnywhere, Category = "Camera")
	float _springArmAirRotationSpeed = 0.2f;

	/**
	* Spring arm orientation tolerance for pitch and roll when the character is not on ground
	*/
	UPROPERTY(EditAnywhere, Category = "Camera")
	float _springArmAirOrientationTolerance = 0.001f;

	/**
	* Time since the spring arm start to re ajuste it's rotation when the character is not on ground
	*/
	float _springArmAirCurrentRotationTime;

	/**
	* Time since the spring arm start to re ajuste it's rotation when the character is not on ground
	*/
	float _springArmResetYawTime;

	/**
	 * Main camera
	 */
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* _mainCamera;

	/// <summary>
	/// Maximum value in degrees of camera yaw orientation control.
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Camera")
	float _maxCameraYaw = 30;

	/// <summary>
	/// Maximum value in degrees of camera pitch orientation control.
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Camera")
	float _maxCameraPitchControlled = 30;

	/// <summary>
	/// Maximum value in degrees of camera orientation control.
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Camera")
	float _cameraOrientationSpeed = 5;

	/// <summary>
	/// Base camera relative orientation
	/// </summary>
	FRotator _baseCameraOrientation;

	/// <summary>
	/// If the player is moving the camera's yaw
	/// </summary>
	bool _isControllingCameraYaw = false;

	/// <summary>
	/// If the player is moving the camera's pitch
	/// </summary>
	bool _isControllingCameraPitch = false;

	// Board ----------------------------------------------------------------------------------------------------
	/**
	 * Default position of the board, used for doing some calculation on the board movement
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board", meta = (AllowPrivateAccess = "true"))
	USceneComponent* _boardContainer;

	/**
	 * Used to know if the board is on the ground or not and avoid some error
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board", meta = (AllowPrivateAccess = "true"))
	USceneComponent* _boardGroundDetector;

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

	/**
	 * Input action to control the camera's yaw
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* _CameraYawControlInputAction;

	/**
	 * Input action to control the camera's pitch
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* _CameraPitchControlInputAction;

	/**
	 * Input action to jump
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _JumpInputAction;

	/**
	 * Input action to shoot
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _ShootInputAction;

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
	* If the player is breaking or not
	*/
	bool _isBraking = false;

	/**
	 * Base target arm length when the character doesn't move
	 */
	float _baseTargetArmLength = 300.f;

	/**
	 * base pitch board value for reset it where the player doesn't try to brake
	 */
	FRotator baseBoardRotation;

	/**
	 * Current player speed
	 */
	float _currentSpeed = 0.f;

	/**
	 * turning speed
	 */
	UPROPERTY(EditAnywhere, Category = "Movement/Turning")
	float _turningSpeed = 2.f;

	/**
	* Turning board roll when the value is as it max
	*/
	UPROPERTY(EditAnywhere, Category = "Movement/Turning")
	float _maxTurningBoardRoll = 50;

	/**
	* Time to swap turning roll when the player change the turning side
	*/
	UPROPERTY(EditAnywhere, Category = "MovementTurning")
	float TimeToSwapTurningBoardRoll = 1.f;

	/**
	* Turning board roll on the previous iteration
	*/
	float _previousTurningBoardRoll;

	/**
	* Board roll used to lerp the stop turning roll rotation
	*/
	float _previousBoardRollForStopTurning;

	/**
	* Current time used for Lerp the swipe of the turning board's roll
	*/
	float _currentTurningBoardRollTime;

	/**
	* Current time used for Lerp the swipe of the stop turning board's roll
	*/
	float _currentStopTurningBoardRollTime;

	/**
	* Time to swipe turning board roll
	*/
	UPROPERTY(EditAnywhere, Category = "Movement/Turning")
	float _timeToSwipeTurningBoardRoll = 0.08f;

	//Air Movement ---------------------------------------------------------------------------------------------------

	/*
	* Speed of vertical air movement
	*/
	UPROPERTY(EditAnywhere, Category = "Movement/Air")
	float _verticalAirSpeed = 0.5f;

	/*
	* Speed of horizontal air movement
	*/
	UPROPERTY(EditAnywhere, Category = "Movement/Air")
	float _horizontalAirSpeed = 2.f;

	/*
	* Speed of horizontal air movement
	*/
	UPROPERTY(EditAnywhere, Category = "Movement/Air")
	float _landingYawTollerance = 85;

	/*
	* Time to reset board container yaw when landing
	*/
	UPROPERTY(EditAnywhere, Category = "Movement/Air")
	float _TimeToResetPitchWhenLanding = 1.f;

	/*
	* Current time used to reset board container yaw when landing
	*/
	float _CurrentTimeToResetPitchWhenLanding;

	/*
	* if the character is flying or not
	*/
	bool _IsFlying;

	//Shoot -----------------------------------------------------------------------------------------------------
	/**
	* base class of the bullet
	*/
	UPROPERTY(EditAnywhere, Category = "Shoot")
	TSubclassOf<ABaseBullet> _BulletClass;

	/**
	* Bullet speed
	* temporary, will remove it when will manage weapons
	*/
	UPROPERTY(EditAnywhere, Category = "Shoot")
	float _BulletSpeed;

	/**
	* Spawner of the bullet
	*/
	UPROPERTY(EditAnywhere, Category = "Shoot")
	USceneComponent* _BulletSpawner;

	/**
	* Bullet damage
	* temporary, will remove it when will manage weapons
	*/
	UPROPERTY(EditAnywhere, Category = "Shoot")
	int _BulletDamage;
	
	//Health -----------------------------------------------------------------------------------------------------
	/**
	* Health of the player
	*/
	UPROPERTY(EditAnywhere, Category = "Health")
	UHealthComponent* _healthComponent;
public:
	//Enemies -----------------------------------------------------------------------------------------------------
	ABaseTargetable* EnemyLocked;

public:
	/**
	 * Default constructor
	 */
	AOverboardPlayer();

private:
	/**
	 * Manage camera pitch control
	 *
	 * @param pInstance input instance used
	 */
	void CameraControlPitch(const FInputActionInstance& pInstance);

	/**
	 * Stop to manage camera pitch control
	 *
	 * @param pInstance input instance used
	 */
	void StopCameraControlPitch(const FInputActionInstance& pInstance);

	/**
	 * Manage camera yaw control
	 *
	 * @param pInstance input instance used
	 */
	void CameraControlYaw(const FInputActionInstance& pInstance);

	/**
	 * Stop to manage camera yaw control
	 *
	 * @param pInstance input instance used
	 */
	void StopCameraControlYaw(const FInputActionInstance& pInstance);

	/**
	 * Manage reset of yaw camera control
	 */
	void ResetCameraControlYaw();

	/** 
	 * Manage vertical movement
	 *
	 * @param pInstance input instance used
	 */
	void VerticalMovement(const FInputActionInstance& pInstance);

	/**
	 * Called to accelerate, deselerate or brake
	 *
	 * @param pValue Value got by the player input axis
	 */
	void ManageAcceleration(float pValue);

	/**
	 * Move the orientation of the character vertically when he is not on the ground
	 *
	 * @param pValue Value got by the player input axis
	 */
	void VerticalAirMovement(float pValue);

	/**
	 * Calculate and apply the acceleration
	 * 
	 * @param pValue Value got by the player input axis
	 */
	void Accelerate(float pValue);

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
	void Brake(float pValue);

	/**
	 * Move the camera to it base position to see the deseleration effect
	 */
	void LerpCameraArmForDeseleration();

	/**
	 * Apply the new speed calculate with acceleration
	 */
	void ApplyNewSpeed();

	/**
	 * Manage horizontal movement
	 *
	 * @param pInstance input instance used
	 */
	void HorizontalMovement(const FInputActionInstance& pInstance);

	/**
	 * Manage vertical movement
	 *
	 * @param pInstance input instance used
	 */
	void Turn(float pValue);

	/**
	 * Move the orientation of the character horizontally when he is not on the ground
	 *
	 * @param pValue Value got by the player input axis
	 */
	void HorizontalAirMovement(float pValue);

	/**
	* Called for set the new board's roll when the character turn
	* 
	* @param pTurningSpeed the turning speed
	*/
	void SetBoardTurningRoll(float pTurningSpeed);

	/**
	 * Called when the player release the turning button
	 *
	 * @param pInstance input instance used
	 */
	void StopTurning(const FInputActionInstance& pInstance);

	/**
	* Called for set the base board's roll when the character stop turning
	*/
	void SetBoardStopTurningRoll();

	/**
	 * Calculate and give the board's Z local position to do the idle movement
	 * 
	 * @param pDeltaTime deltatime between two ticks
	 * 
	 * @return the new board's Z position
	 */
	double GetBoardZPositionForIdle(float pDeltaTime);

	/**
	 * Calculate and give the board's Z local position to do the acceleration movement
	 *
	 * @param pOldZ Z local position of the board before the calculation
	 * 
	 * @return the new board's Z position
	 */
	double GetBoardZPositionForAcceleration(double pOldZ);

	/**
	 * Set the arm orientation in fonction of the board container orientation when the character is on the ground
	 *
	 * @param pDeltaTime deltatime between two ticks
	 */
	void SetGroundedArmOrientation(float pDeltaTime);

	/**
	 * Set the arm orientation in fonction of the board container orientation when the character doesn't touch the ground
	 *
	 * @param pDeltaTime deltatime between two ticks
	 */
	void SetAirArmOrientation(float pDeltaTime);

	/**
	 * Set the arm orientation in fonction of the board container orientation
	 *
	 * @param pWantedRotation the final orientation wanted
	 * @param pTotalTime time calculated since the start of the movement
	 * @param pDeltaTime deltatime between two ticks
	 * @param pTolerance error telerance
	 * @param pSpeed Lerp speed
	 * 
	 * @return the totaltime updated
	 */
	float SetArmOrientation(FRotator pWantedRotation, float pTotalTime, float pDeltaTime, float pTolerance, float pSpeed);

	/**
	* Set the board pitch
	* 
	* @param pPitch new pitch of the board to set
	*/
	void SetBoardPitch(float pPitch);

	/**
	* Set the board roll
	*
	* @param pPitch new roll of the board to set
	* @param if we lerp the roll rotation or just set it
	* @param previous roll value for lerp
	* @param pCurrentTime used for lerp
	* @param pMaxTime used for lerp
	* 
	* @return true if the final rotation is set, false otherwise
	*/
	bool SetBoardRoll(float pRoll, bool pLerp = false, float pPreviousRoll = 0, float pCurrentTime = 0.f, float pMaxTime = 0.f);

	/**
	* When the player land on the ground
	* @param the actor hit to land
	*/
	void Landing(const FHitResult& pHit);

	/**
	* Check if the enemy locked is still in view
	*
	* @param pEnemies new enemies in view
	*
	* @return true if the enemy is valid, false instead
	*/
	bool CheckEnemyLockedValididy(TArray<AActor*> pEnemies);

	/**
	* Update the enemy lock taking the closest by default
	*
	* @param pEnemies new enemies in view
	*/
	void UpdateEnemyLocked(TArray<AActor*> pEnemies);
protected:
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

	/**
	* When the player just landed
	* 
	*  @param Hit the ground in landed on
	*/
	virtual void Landed(const FHitResult& Hit) override;

	/**
	* When the array of enemies in view is updated
	* 
	* @param pEnemies new enemies in view
	*/
	void EnemiesInViewUpdated(TArray<AActor*> pEnemies);

	/**
	* Update the enemy lock
	*
	* @param pEnemy new enemy to lock
	*/
	void UpdateEnemyLocked(ABaseTargetable* pEnemy);

	/**
	* Shoot on a target
	*/
	void Shoot();
};
