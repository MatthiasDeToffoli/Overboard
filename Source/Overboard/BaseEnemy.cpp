// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include <Kismet/GameplayStatics.h>
#include "OverboardPlayer.h"
#include "ScreenLogger.h"

ABaseEnemy::ABaseEnemy()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
	
	_CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision's box");
	if (_mainContainer)
	{
		_CollisionComponent->SetupAttachment(_mainContainer);
	}

	_CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnBeginOverlap);
}

void ABaseEnemy::OnBeginOverlap(UPrimitiveComponent* pOverlappedComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 pOtherBodyIndex, bool pFromSweep, const FHitResult& pSweepResult)
{
	if (pOtherActor->IsA(AOverboardPlayer::StaticClass()))
	{
		AOverboardPlayer* lTarget = Cast<AOverboardPlayer>(pOtherActor);

		if (lTarget)
		{
			UGameplayStatics::ApplyDamage(lTarget, _damage, nullptr, this, nullptr);
		}

		Destroy();
	}
}