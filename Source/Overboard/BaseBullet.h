// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBullet.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

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
	USceneComponent* _GraphismContainer;

	/**
	* Bullet's graphism
	*/
	UPROPERTY(EditAnywhere, Category = "Graphism")
	UStaticMeshComponent* _Graphism;

	/**
	* Sphere collision component 
	*/
	UPROPERTY(EditAnywhere, Category = "Collision")
	USphereComponent* _CollisionComponent;

	/**
	* Projectile movement
	*/
	UPROPERTY(EditAnywhere, Category = "Movement")
	UProjectileMovementComponent* _ProjectileMovement;

	int _damage;

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
	* @param pHitActor the actor hit
	* @param pNormalImpulse the normal impulse of the hit
	* @param pHit the hit result
	*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* pHitComponent, AActor* pHitActor, UPrimitiveComponent* pOtherComponent, FVector pNormalImpulse, const FHitResult& pHit);

};
