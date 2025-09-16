#pragma once

#include <AIController.h>
#include "CoreMinimal.h"
#include "BaseTargetable.h"
#include "BaseEnemy.generated.h"

class USphereComponent;

/**
 * Parent of all enemies class
 */
UCLASS()
class OVERBOARD_API ABaseEnemy : public ABaseTargetable
{
	GENERATED_BODY()

private:
	/**
	* Damage of the enemy
	*/
	float _damage = 10.f;

public:
	/// <summary>
	/// if the enemy is stupid, it will not avoid walls
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool isStupid;

	/// <summary>
	/// Wall avoiding distance
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float wallAvoidingDistance = 300.f;

	/// <summary>
	/// Offset to see the aread the enemy check to avoid walls
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float avoidingOffsetValue = 50.f;

	/// <summary>
	/// Movement speed
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float speed = 6000.f;

	/**
	* Sphere collision component
	*/
	UPROPERTY(EditAnywhere, Category = "Collision")
	USphereComponent* _CollisionComponent;

public:
	/**
	* Default constructor
	*/
	ABaseEnemy();

public:
	/*
	* Call when the actor overlap something
	*
	* @param pOverlappedComp the component caller
	* @param pOtherActor the actor overlapped
	* @param pOtherComp the component overlapped
	* @param pOtherBodyIndex the actor overlapped's body index
	* @param pFromSweep if it used sweep
	* @param pSweepResult the sweep result
	*/
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* pOverlappedComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 pOtherBodyIndex, bool pFromSweep, const FHitResult& pSweepResult);
};
