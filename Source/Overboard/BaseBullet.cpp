#include "BaseBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "ActorBuilder.h"
#include "BaseTargetable.h"
#include <Kismet/GameplayStatics.h>
#include "OverboardPlayer.h"

ABaseBullet::ABaseBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	_CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision's box");
	RootComponent = _CollisionComponent;
    _GraphismContainer = UActorBuilder::CreateSubObjects<USceneComponent>(this, RootComponent, "Graphism container");
    _Graphism = UActorBuilder::CreateSubObjects<UStaticMeshComponent>(this, _GraphismContainer, "Graphism");
    
	_CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseBullet::OnHit);
    _ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile movement");

}

void ABaseBullet::Configure(FVector pStartLocation, FVector pTargetLocation, float pSpeed, int pDamage)
{
	_ProjectileMovement->Velocity = (pTargetLocation - pStartLocation).GetSafeNormal() * pSpeed;
	_damage = pDamage;
}

void ABaseBullet::OnHit(UPrimitiveComponent* pHitComponent, AActor* pHitActor, UPrimitiveComponent* pOtherComponent, FVector pNormalImpulse, const FHitResult& pHit)
{
	if (pHitActor != this && !pHitActor->IsA(AOverboardPlayer::StaticClass()))
	{
		ABaseTargetable* lTarget = Cast<ABaseTargetable>(pHitActor);

		if (lTarget)
		{
			UGameplayStatics::ApplyDamage(lTarget, _damage, nullptr, this, nullptr);
		}

		Destroy();
	}
}
