// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "ActorBuilder.h"
#include "BaseTargetable.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ABaseBullet::ABaseBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    _GraphismContainer = UActorBuilder::CreateSubObjects<USceneComponent>(this, RootComponent, "Graphism container");
    _Graphism = UActorBuilder::CreateSubObjects<UStaticMeshComponent>(this, _GraphismContainer, "Graphism");

    _CollisionComponent = UActorBuilder::CreateSubObjects<USphereComponent>(this, RootComponent, "Collision's box");;
	_CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseBullet::OnHit);
    _ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile movement");

}

// Called every frame
void ABaseBullet::Configure(FVector pStartLocation, FVector pTargetLocation, float pSpeed, int pDamage)
{
	_ProjectileMovement->Velocity = (pTargetLocation - pStartLocation).GetSafeNormal() * pSpeed;
	_damage = pDamage;
}

void ABaseBullet::OnHit(UPrimitiveComponent* pHitComponent, AActor* pHitActor, UPrimitiveComponent* pOtherComponent, FVector pNormalImpulse, const FHitResult& pHit)
{
	ABaseTargetable* lTarget = Cast<ABaseTargetable>(pHitActor);

	if (lTarget)
	{
		UGameplayStatics::ApplyDamage(lTarget, _damage, nullptr, this, nullptr);
	}

	Destroy();
}

