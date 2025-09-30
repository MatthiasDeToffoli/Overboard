#include "BaseBullet.h"
#include "ActorBuilder.h"
#include "BaseTargetable.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include "OverboardPlayer.h"

ABaseBullet::ABaseBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	collisionComponent_ = CreateDefaultSubobject<USphereComponent>("Collision's box");
	RootComponent = collisionComponent_;
    graphismContainer_ = UActorBuilder::CreateSubObjects<USceneComponent>(this, RootComponent, "Graphism container");
    graphism_ = UActorBuilder::CreateSubObjects<UStaticMeshComponent>(this, graphismContainer_, "Graphism");
    
	collisionComponent_->OnComponentHit.AddDynamic(this, &ABaseBullet::OnHit);
    projectileMovement_ = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile movement");

}

void ABaseBullet::Configure(FVector pStartLocation, FVector pTargetLocation, float pSpeed, int pDamage)
{
	projectileMovement_->Velocity = (pTargetLocation - pStartLocation).GetSafeNormal() * pSpeed;
	damage_ = pDamage;
}

void ABaseBullet::OnHit(UPrimitiveComponent* pHitComponent, AActor* pHitActor, UPrimitiveComponent* pOtherComponent, FVector pNormalImpulse, const FHitResult& pHit)
{
	if (pHitActor != this && !pHitActor->IsA(AOverboardPlayer::StaticClass()))
	{
		ABaseTargetable* lTarget = Cast<ABaseTargetable>(pHitActor);

		if (lTarget)
		{
			UGameplayStatics::ApplyDamage(lTarget, damage_, nullptr, this, nullptr);
		}

		Destroy();
	}
}
