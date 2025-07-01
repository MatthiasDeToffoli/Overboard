// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include <Kismet/GameplayStatics.h>
#include "OverboardPlayer.h"

ABaseEnemy::ABaseEnemy()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

	_CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision's box");
	_CollisionComponent->SetupAttachment(_mainContainer);
	if (_mainContainer)
	{
		_CollisionComponent->SetupAttachment(_mainContainer);
	}

	_CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseEnemy::OnHit);
}

void ABaseEnemy::OnHit(UPrimitiveComponent* pHitComponent, AActor* pHitActor, UPrimitiveComponent* pOtherComponent, FVector pNormalImpulse, const FHitResult& pHit)
{
	if (pHitActor->IsA(AOverboardPlayer::StaticClass()))
	{
		AOverboardPlayer* lTarget = Cast<AOverboardPlayer>(pHitActor);

		if (lTarget)
		{
			UGameplayStatics::ApplyDamage(lTarget, _damage, nullptr, this, nullptr);
		}

		Destroy();
	}

}