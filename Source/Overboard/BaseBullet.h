#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBullet.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

/*
* Parent of all bullets class
*/
UCLASS()
class OVERBOARD_API ABaseBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	* Default constructor
	*/
	ABaseBullet();

private:
	/**
	* Container of the bullet's graphism
	*/
	UPROPERTY(EditAnywhere, Category = "Graphism")
	USceneComponent* graphismContainer_;

	/**
	* Bullet's graphism
	*/
	UPROPERTY(EditAnywhere, Category = "Graphism")
	UStaticMeshComponent* graphism_;

	/**
	* Sphere collision component 
	*/
	UPROPERTY(EditAnywhere, Category = "Collision")
	USphereComponent* collisionComponent_;

	/**
	* Projectile movement
	*/
	UPROPERTY(EditAnywhere, Category = "Movement")
	UProjectileMovementComponent* projectileMovement_;

	/**
	* Damage of the bullet
	*/
	int damage_;

public:	
	/**
	* Configure the bullet to shoot the target
	* 
	* @param pStartLocation Location of the bullet spawner
	* @param pTargetLocation Location of the target to shoot
	* @param pSpeed speed of the bullet
	* @param pDamage damage of the bullet
	*/
	void Configure(FVector pStartLocation, FVector pTargetLocation, float pSpeed, int pDamage);

	/*
	* Call when the actor hit something
	* 
	* @param pHitComponent the component hit
	* @param pHitActor the actor hit
	* @param pOtherComponent the other component involved in the hit
	* @param pNormalImpulse the normal impulse of the hit
	* @param pHit the hit result
	*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* pHitComponent, AActor* pHitActor, UPrimitiveComponent* pOtherComponent, FVector pNormalImpulse, const FHitResult& pHit);

};
